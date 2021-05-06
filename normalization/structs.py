from datetime import datetime
from itertools import groupby
import pandas as pd
from typing import List, Optional, Dict, Union
from pydantic import validator, Field
from pathlib import Path
import json


from pydantic import BaseModel


class BaseInstanceModel(BaseModel):
    @classmethod
    def create_instance(cls, data, instance):

        cls.data = data
        return cls(**instance)

    def __getitem__(self, item):
        return getattr(self,item)


class String(BaseInstanceModel):
    name: str

class InvertMppt(BaseInstanceModel):
    name: str
    string: Optional[List[String]]
    @validator("string", pre=True)
    def create_mppt(cls, v):
        return  [String.create_instance(cls.data, _string) for _string in v]


class Inverter(BaseInstanceModel):
    name: str
    mppt: Optional[List[InvertMppt]]

    @validator("mppt", pre=True)
    def create_mppt(cls, v):
        return  [InvertMppt.create_instance(cls.data, _mppt) for _mppt in v]

class Plant(BaseInstanceModel):
    inverter: Optional[List[Inverter]]

    @validator("inverter", pre=True)
    def create_inverter(cls, v):
        return  [Inverter.create_instance(cls.data, _inverter) for _inverter in v]

    @property
    def mppt(self):
        return [mppt for _inverter in self.inverter for mppt in _inverter.mppt]

    @property 
    def inverter_mppt_ids(self):
        return [mppt.name for mppt in self.inverter_mppt]

    def get_inverter(self, name: str):
        return [_inverter for _inverter in self.inverter if _inverter.name == name]

    @property
    def string(self):
        return [string for _inverter in self.inverter
                    for mppt in _inverter.mppt 
                    for string in mppt.string]
        
    def get_df(self,level:str, df: pd.DataFrame,
        id_col: str="id", by:str="date")-> dict:
        if getattr(self, level):
            level_lst = getattr(self, level)
        else:
            raise KeyError("%s Not exist" %level)
        return {
            id.name : df[df[id_col].str.contains(id.name)].groupby([by]).sum() for id in level_lst
        } 


class PlantFactory:
    def __init__(self, data, root: str = "plant") -> None:
        self.data = data
        self.plant = None
        self.create(root)

    def create(self, root: str) -> None:
        if not self.plant:
            self.plant = Plant.create_instance(self.data, self.data[root])

    def get_plant(self) -> Plant:
        return self.plant


if __name__ == "__main__":
    path = Path("data/tenrev.json")

    with open(path, 'rb') as f:
        data = json.load(f)
    factory = PlantFactory(data)
    plant = factory.get_plant()

    df = pd.read_csv("data/mppt.csv", sep=";")
    inverter = pd.read_csv("data/inverter.csv", sep=";")

    level_dct = plant.get_df("inverter", df)
    df_inverter = level_dct.get("2110209488")["power"]






