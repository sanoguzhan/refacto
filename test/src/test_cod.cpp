// #include "test_header.hpp"

// class CODTest : public TestTimer {};
// /* Test for Decompress, compress and clean Operator
//     - Test Decompressdir with tar, zip, and gz extension
//     - Test for Cleanfile functor
// */

// TEST(CompressOrDecompress, TestZIP) {
//     DECOMPRESSDIR()("tests/test_data/gz/");

//     CLEANFILES()("tests/test_data/gz/", "*.csv");
//     ASSERT_EQ(find_ext("tests/test_data/gz/"), "gz");

//     //   DECOMPRESSDIR()("tests/test_data/zips/");
//     //   CLEANFILES()("tests/test_data/zips/", "Mean*");
//     //   CLEANFILES()("tests/test_data/zips/", "Log*");
// }

// TEST(CompressOrDecompress, TestListDir) {
//     ASSERT_EQ("gz", find_ext("tests/test_data/gz/"));
//     ASSERT_EQ("tar", find_ext("tests/test_data/tar/"));
//     ASSERT_EQ("zip", find_ext("tests/test_data/zips/"));
//     try {
//         find_ext("t.");
//     } catch (std::exception &ex) {
//         EXPECT_EQ("Cannot open t.", (std::string)ex.what());
//     }
// }

// int main(int argc, char *argv[]) {
//     testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
