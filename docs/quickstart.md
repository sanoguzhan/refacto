# Quick Start Tutorial

## CSV Parser

Unstructred files are transform to more uniform format with given ordered-conditions.
Condition should follow the order as if there is a conditional values, this should come first in the group of keys
if any id is not exist in the data then ids condition should be the first in grouped keys

The Constructer takes arbitary number of ordered-dictionaries and creates the Entity object each condition
Operator () is overload and expect the directory path of files as string, deliemeter, and the rows to be skipped

_Example 0:_

In some cases, column name include each key such as "A_mppt_power_watt" A as key of the mppt as all column includes variable of it. In this cases grouping condition can be used with regex to transform the data.
In the condition, first group belong to id and second condition for column variable

```python
{"key": "inverter",
        "name":"Mppt_power",
        "orient": "row",
        "type" : "group",
        "row" : "0",
        "value_begin" : "1",
        "condition": {
        "id": "(.*)_string_current_amper",
        "name": ".*(_string_current_amper)"
            }
        }
```

_Example 1:_  
Here the key is the condition and data parsed based on the key condition
So for every variable found with given name, Pac column will be inserted to data

```python
{"key": "inverter",
        "name":"Pac",
        "orient": "row",
        "type" : "series",
        "row" : "1",
        "value_begin" : "3",
        "condition": {
            "name" : "^2001.*98$",
            "orient": "row",
            "row": "0"
        }
```

_Example 2:_

When the data does not include any keys in the header, the key should be parsed manually
In the example below, the key location is given as, INV header name is located at row 0 column of INV header is 0 where the iterator begins for search and Value begin is the location of each value

_Data Example:_

| Date     | Foo | INV | Foo | INV |
| -------- | :-: | --: | :-: | --: |
| 12.12.12 | 10  |   1 |  0  |   2 |

The data includes multiple keys so keyword multi is given

**Note**: Constructor takes arbitary number of variable,However for key of group id specification must be the first value.

```python
{"key": "inverter",
        "name":"INV",
        "orient": "row",
        "type" : "ids",
        "row" : "0",
        "column" : "0",
        "value_begin":"1",
        "keyword": "multi"
        }
```

_Example 3:_

As another option, file name can be parsed if ids are not exist in the dataset.
It will map each value here from the opened file name,
so the file only includes variable ones since we have only one key

```python
{"key": "inverter",
        "name":"INV",
        "orient": "row",
        "type" : "ids",
        "keyword": "file_name"
        }

```

## XMLParser

Xml is tree-based data structure so the key must be parent name
Similar to CSVParser XMLParser constructer takes arbitary number of Dictionary for conditonal parsing

After Object instantiated, overloaded () operator should be called with directory path as a string,
ans the root name of the file

Here, the id is used to map values for each row of Pac
XML files include various level of data such as Mean, Median, Max, Min. In this example Mean is taken as value
Unlike CSVParser, XMLParser parameter's order not important

```python
{"name": "inverter",
        "node":"Key",
        "key":"Pac",
        "degree":"Mean"}

```

There are 3 types of input type fpr XMLParser:

1 - Single
2 - Multi
3 - Group

### Single Input:

Values can be a single entity from XML Tree, in this case. All files searched and values converted to a table. Single search value example below:

```python
{"name": "inverter",
        "node":"Key",
        "key":"Pac",
        "degree":"Mean",
        "type":"single",
        "output":"foobar"}
```

### Multi Input:

Values can be multi columns from XML Tree, in this case, values are searched with given regex and a column is created per found vairable. All values found with key condition are stored under column name and converted to a table when to_csv is invoked.

Example for multi search:

```python
 {"name": "inverter",
            "node":"Key",
            "key":"^(.*?).Ms.Amp",
            "degree":"Mean",
            "type": "multi"}
```

### Group Input:

Values can be a group of colu from XML Tree, in this case, values are grouped based on given condition
Condition must include id and name keys
for each condition, value column created

Example:
```python
         {"name": "inverter",
            "node":"Key",
            "key":"^(.*?)Uac",
            "degree":"Mean",
            "type": "group",
            "condition":{
                "id": "(.*)Uac",
                "name" :  ".*(Uac)"
            }}
```
## Decompressdir

Uses shell command to decompress files in gzip, zip or tar extension
Extension is searched in the given directory so the directory should be uniform

_Example:_

```python
            Decompressdir()("parsing/core/tests/test_data/gz/")
```

## Compressdir

Uses shell command to compress files in zip format

_Example:_

```python
    Compressdir()("parsing/core/tests/test_data/gz/", "test")
```

### Cleandir

Uses shell command to delete files in zip format.

**NOT:** Careful when you use it you may delete important files

It takes an extension of file to be deleted as safety guard

_Example:_

```
    Cleandir()("src/core/tests/test_data/gz/", "*.csv"
```
