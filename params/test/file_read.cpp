/*
 * Copyright (C) 1998-2014 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include "alps/params.hpp"
#include "gtest/gtest.h"
#include "alps/utilities/temporary_filename.hpp"

#include "boost/lexical_cast.hpp"

#include <fstream>

// Scalar param read (except bool which is tested separately)
TEST(param, ScalarParamRead) {
   //create a file name
   std::string pfilename(alps::temporary_filename("pfile"));

   // Generate INI file
   int param_int=1234;
   double param_double=1.125;
   float param_float=2.25;
   {
     std::ofstream pfile(pfilename.c_str());
     pfile << std::boolalpha;
     pfile << "int = " << param_int << std::endl;
     pfile << "double = " << param_double << std::endl;
     pfile << "float = " << param_float << std::endl;
   }

   // Imitate the command line args
   const int argc=2;
   const char* argv[2]={"THIS_PROGRAM",0};
   argv[1]=pfilename.c_str();

   //define the parameters
   alps::params p(argc,argv);
   p.description("This is a test program").
       define<int>("int","int parameter").
       define<double>("double","double parameter").
       define<float>("float","float parameter");
   
   // read the parameters
   
   int param_int_rd=p["int"];
   double param_double_rd=p["double"];
   float param_float_rd=p["float"];

   // verify the parameters
   EXPECT_EQ(param_int_rd, param_int);
   EXPECT_NEAR(param_double_rd,param_double,1.E-12);
   EXPECT_NEAR(param_float_rd,param_float,1.E-12);
}    

// Boolean param read
TEST(param, BoolParamRead) {
   //create a file name
   std::string pfilename(alps::temporary_filename("pfile"));

   // Generate INI file
   {
     std::ofstream pfile(pfilename.c_str());
     pfile <<
         "ok = 1\n"
         "nok = 0\n"
         "truepar = true\n"
         "falsepar = false\n"
         "truepar1 = TruE\n"
         "falsepar1 = FAlse\n"
         "yespar = YeS\n"
         "nopar = No\n";
   }

   // Imitate the command line args
   const int argc=2;
   const char* argv[2]={"THIS_PROGRAM",0};
   argv[1]=pfilename.c_str();

   //define the parameters
   alps::params p(argc,argv);
   p.description("This is a test program").
       define<bool>("ok","ok").
       define<bool>("nok","nok").
       define<bool>("truepar","truepar").
       define<bool>("falsepar","falsepar").
       define<bool>("truepar1","truepar1").
       define<bool>("falsepar1","falsepar1").
       define<bool>("yespar","yespar").
       define<bool>("nopar","nopar");
   
   // read the parameters and compare
   EXPECT_EQ(bool(p["ok"]),true);
   EXPECT_EQ(bool(p["nok"]),false);
   EXPECT_EQ(bool(p["truepar"]),true);
   EXPECT_EQ(bool(p["falsepar"]),false);
   EXPECT_EQ(bool(p["truepar1"]),true);
   EXPECT_EQ(bool(p["falsepar1"]),false);
   EXPECT_EQ(bool(p["yespar"]),true);
   EXPECT_EQ(bool(p["nopar"]),false);
}

    
// String param read (stripping spaces, stripping quotes)
TEST(param, StringParamRead) {
   //create a file name
   std::string pfilename(alps::temporary_filename("pfile"));

   // Have some sample strings
   std::string basic="hello-world";
   std::string no_tl_spaces="hello, \"my beautiful\" world"; // no trailing/leading spaces
   std::string with_tl_spaces="   " + no_tl_spaces + " ";
   std::string with_tl_spaces_quoted="\"" + with_tl_spaces + "\"";
   std::string with_tl_spaces_quoted2="\"\"" + with_tl_spaces + "\"\"";
   std::string ends_with_quote="Hello, \"my world\"";
   std::string starts_with_quote="\"Hello, world!\", she said.";

   // Generate INI file
   {
     std::ofstream pfile(pfilename.c_str());
     pfile << "basic = " << basic << std::endl;
     pfile << "no_tl_spaces = " << no_tl_spaces << std::endl;
     pfile << "with_tl_spaces = " << with_tl_spaces << std::endl;
     pfile << "with_tl_spaces_quoted = " << with_tl_spaces_quoted << std::endl;
     pfile << "with_tl_spaces_quoted2 = " << with_tl_spaces_quoted2 << std::endl;
     pfile << "ends_with_quote = " << ends_with_quote << std::endl;
     pfile << "starts_with_quote = " << starts_with_quote << std::endl;
   }

   // Imitate the command line args
   const int argc=2;
   const char* argv[2]={"THIS_PROGRAM",0};
   argv[1]=pfilename.c_str();

   //define the parameters
   alps::params p(argc,argv);
   p.description("This is a test program").
       define<std::string>("basic","basic string parameter").
       define<std::string>("no_tl_spaces","No trailing or leading spaces").
       define<std::string>("with_tl_spaces","with trailing and leading spaces").
       define<std::string>("with_tl_spaces_quoted","with t/l spaces, quoted").
       define<std::string>("with_tl_spaces_quoted2","with t/l spaces/quotes, quoted").
       define<std::string>("ends_with_quote","ending with quote").
       define<std::string>("starts_with_quote","starting with quote");
   
   // read the parameters
   std::string basic_rd=p["basic"];
   std::string no_tl_spaces_rd=p["no_tl_spaces"];
   std::string with_tl_spaces_rd=p["with_tl_spaces"];
   std::string with_tl_spaces_quoted_rd=p["with_tl_spaces_quoted"];
   std::string with_tl_spaces_quoted2_rd=p["with_tl_spaces_quoted2"];
   std::string ends_with_quote_rd=p["ends_with_quote"];
   std::string starts_with_quote_rd=p["starts_with_quote"];

   // verify the parameters
   EXPECT_EQ(basic_rd, basic);
   EXPECT_EQ(no_tl_spaces_rd, no_tl_spaces);
   EXPECT_EQ(with_tl_spaces_rd, no_tl_spaces);
   EXPECT_EQ(with_tl_spaces_quoted_rd, with_tl_spaces);
   EXPECT_EQ(with_tl_spaces_quoted2_rd, with_tl_spaces_quoted);
   EXPECT_EQ(ends_with_quote_rd,ends_with_quote);
   EXPECT_EQ(starts_with_quote_rd,starts_with_quote);
}    

// Various spaces around the values
template <typename T>
void Test_spaces_around(T my_param)
{
   //create a file name
   std::string pfilename(alps::temporary_filename("pfile"));

   // Generate INI file
   {
     std::ofstream pfile(pfilename.c_str());
     pfile << "my_param1=" << my_param << std::endl; // no spaces
     pfile << "my_param2 =" << my_param << std::endl; // space before '='
     pfile << "my_param3= " << my_param << std::endl; // space before value
     pfile << "my_param4 = " << my_param << std::endl; // spaces around '='
     pfile << "my_param5 = " << my_param << "  " << std::endl; // spaces around '=' and after value
   }

   // Imitate the command line args
   const int argc=2;
   const char* argv[2]={"THIS_PROGRAM",0};
   argv[1]=pfilename.c_str();

   //define the parameters
   alps::params p(argc,argv);
   p.description("This is a test program").
       template define<T>("my_param1","no spaces").
       template define<T> ("my_param2","space before =").
       template define<T>("my_param3","space after =").
       template define<T>("my_param4","spaces around =").
       template define<T>("my_param5","trailing spaces");
   
   // read the parameters
   T my_param1_rd=p["my_param1"];
   T my_param2_rd=p["my_param2"];
   T my_param3_rd=p["my_param3"];
   T my_param4_rd=p["my_param4"];
   T my_param5_rd=p["my_param5"];

   // verify the parameters
   EXPECT_EQ(my_param1_rd, my_param);
   EXPECT_EQ(my_param2_rd, my_param);
   EXPECT_EQ(my_param3_rd, my_param);
   EXPECT_EQ(my_param4_rd, my_param);
   EXPECT_EQ(my_param5_rd, my_param);
}

#define MakeTest(typ,typ_human,val)             \
    TEST(param, SpacesRead ## typ_human) { Test_spaces_around< typ > ( val ); } 

MakeTest(int, INT, 1234)
MakeTest(double, DOUBLE, 4.25)
MakeTest(std::string, STRING, "hello")

#undef MakeTest

// Sectioned ini file
TEST(param, SectionedFile) {
   //create a file name
   std::string pfilename(alps::temporary_filename("pfile"));

   // Generate INI file
   int param_int1=1234;
   int param_int2=5678;
   double param_double1=1.125;
   double param_double2=2.250;
   {
     std::ofstream pfile(pfilename.c_str());
     pfile << "[SECTION_ONE]" << std::endl;
     pfile << "int = " << param_int1 << std::endl;
     pfile << "double = " << param_double1 << std::endl;

     pfile << "[SECTION_TWO]" << std::endl;
     pfile << "int = " << param_int2 << std::endl;
     pfile << "double = " << param_double2 << std::endl;
   }

   // Imitate the command line args
   const int argc=2;
   const char* argv[2]={"THIS_PROGRAM",0};
   argv[1]=pfilename.c_str();

   //define the parameters
   alps::params p(argc,argv);
   p.description("This is a test program").
       define<int>("SECTION_ONE.int","int1 parameter").
       define<double>("SECTION_ONE.double","double1 parameter").
       define<int>("SECTION_TWO.int","int2 parameter").
       define<double>("SECTION_TWO.double","double2 parameter");
   
   // read the parameters
   int param_int1_rd=p["SECTION_ONE.int"];
   double param_double1_rd=p["SECTION_ONE.double"];
   int param_int2_rd=p["SECTION_TWO.int"];
   double param_double2_rd=p["SECTION_TWO.double"];

   // verify the parameters
   EXPECT_EQ(param_int1_rd, param_int1);
   EXPECT_EQ(param_int2_rd, param_int2);
   EXPECT_NEAR(param_double1_rd,param_double1,1.E-12);
   EXPECT_NEAR(param_double2_rd,param_double2,1.E-12);
}    


// Comments and empty lines
TEST(param, CommentsInFile) {
   //create a file name
   std::string pfilename(alps::temporary_filename("pfile"));

   // Generate INI file
   int param_int1=1234;
   int param_int2=5678;
   double param_double1=1.125;
   double param_double2=2.250;
   {
     std::ofstream pfile(pfilename.c_str());
     pfile << "int1 = " << param_int1 << std::endl;
     pfile << std::endl;
     pfile << "double1 = " << param_double1 << std::endl;

     pfile << "# This is a comment line" << std::endl;
     pfile << "int2 = " << param_int2 << std::endl;
     pfile << "double2 = " << param_double2 << std::endl;
   }

   // Imitate the command line args
   const int argc=2;
   const char* argv[2]={"THIS_PROGRAM",0};
   argv[1]=pfilename.c_str();

   //define the parameters
   alps::params p(argc,argv);
   p.description("This is a test program").
       define<int>("int1","int1 parameter").
       define<double>("double1","double1 parameter").
       define<int>("int2","int2 parameter").
       define<double>("double2","double2 parameter");
   
   // read the parameters
   int param_int1_rd=p["int1"];
   double param_double1_rd=p["double1"];
   int param_int2_rd=p["int2"];
   double param_double2_rd=p["double2"];

   // verify the parameters
   EXPECT_EQ(param_int1_rd, param_int1);
   EXPECT_EQ(param_int2_rd, param_int2);
   EXPECT_NEAR(param_double1_rd,param_double1,1.E-12);
   EXPECT_NEAR(param_double2_rd,param_double2,1.E-12);
}    

// Incorrect input (garbage lines)
TEST(param, GarbageInFile) {
   //create a file name
   std::string pfilename(alps::temporary_filename("pfile"));

   // Generate INI file with garbage
   int param_int1=1234;
   int param_int2=5678;
   const std::string garbage="Not comment, not empty line";
   {
     std::ofstream pfile(pfilename.c_str());
     pfile << "int1 = " << param_int1 << std::endl;
     pfile << garbage << std::endl;
     pfile << "int2 = " << param_int2 << std::endl;
   }

   // Imitate the command line args
   const int argc=2;
   const char* argv[2]={"THIS_PROGRAM",0};
   argv[1]=pfilename.c_str();

   //define the parameters
   alps::params p(argc,argv);
   p.description("This is a test program").
       define<int>("int1","int1 parameter").
       define<int>("int2","int2 parameter");
   
   // read the parameters
   try {
       int param_int1_rd=p["int1"];
       FAIL();
       int param_int2_rd=p["int2"];
   } catch (boost::program_options::invalid_config_file_syntax& ex) {
       SUCCEED();
       EXPECT_TRUE(std::string(ex.what()).find(garbage) != std::string::npos);
       // std::cout << "Exception: " << ex.what() << std::endl;
   }
}    

// Incorrect input (wrong values)

/// Tests that an exception of type E is thrown if a deliberately wrong value strval of type T is supplied.
template <typename T, typename E>
void WrongTypeTest(const std::string& strval)
{
   //create a file name
   std::string pfilename(alps::temporary_filename("pfile"));

   // Generate INI file with wrong-type values
   {
     std::ofstream pfile(pfilename.c_str());
     pfile << "param = " << strval << std::endl;
   }

   // Imitate the command line args
   const int argc=2;
   const char* argv[2]={"THIS_PROGRAM",0};
   argv[1]=pfilename.c_str();

   //define the parameters
   alps::params p(argc,argv);
   p.description("This is a test program").
       define<T>("param","some parameter");
   
   // read the parameter
   try {
       T param_rd=p["param"];
       FAIL();
   } catch (E& ex) {
       SUCCEED();
       // expect that the "wrong" string is somewhere in the exception message
       EXPECT_TRUE(std::string(ex.what()).find(strval)!=std::string::npos);
       // std::cout << "Exception: " << ex.what() <<  std::endl;
   }
}    


TEST(param, WrongValuesInFile) {
    WrongTypeTest<int,boost::program_options::invalid_option_value>("123.45");
    WrongTypeTest<double,boost::program_options::invalid_option_value>("123.45 is not correct");
    WrongTypeTest<bool,boost::program_options::invalid_bool_value>("false true");
}

// Anonymous namespace for service functions
namespace {
  // Service function: output vector as a separated list
  template <typename T>
  std::ostream& print_vec(std::ostream& os, const std::vector<T>& vec, const char* sep)
  {
    typedef std::vector<T> vec_type;
    if (vec.empty()) return os;
    for (typename vec_type::const_iterator it=vec.begin(), end=vec.end(); ;) {
      os << *(it++);
      if (it == end) break;
      os << sep;
    }
    return os;
  }
}


// Vector param read
TEST(param, VectorRead) {
   //create a file name
   std::string pfilename(alps::temporary_filename("pfile"));

   //have some vector parameters
   int intvec_param0[]={10, 20, 30, 40};
   double dblvec_param0[]={1.5, 2.25, 4.125};
   bool boolvec_param0[]={true, false, false, true};

   //have some delimiters
   const char* delims[]={ " ", "," , ", " };
   const int ndelims=sizeof(delims)/sizeof(*delims);

   //convert basic type arrays to std::vector
#define ARRAY_TO_VECTOR(typ, vecval, arrval) \
   std::vector<typ> vecval(arrval, arrval+sizeof(arrval)/sizeof(*arrval))

   ARRAY_TO_VECTOR(int, intvec_param, intvec_param0);
   ARRAY_TO_VECTOR(double, dblvec_param, dblvec_param0);
   ARRAY_TO_VECTOR(bool, boolvec_param, boolvec_param0);

#undef ARRAY_TO_VECTOR
   
   //create ini file with vector parameters and various delimiters
   {
     std::ofstream pfile(pfilename.c_str());
     boolalpha(pfile);

#define MAKE_PARAM(par) \
     for (int i=0; i<ndelims; ++i) { pfile << #par << i << " = "; print_vec(pfile,par,delims[i]); pfile << std::endl; }

     MAKE_PARAM(intvec_param);
     MAKE_PARAM(dblvec_param);
     MAKE_PARAM(boolvec_param);
#undef MAKE_PARAM

   }

   // Imitate the command line args
   const int argc=2;
   const char* argv[2]={"THIS_PROGRAM",0};
   argv[1]=pfilename.c_str();

   //define the parameters, for each delimiter
   alps::params p(argc,argv);
   p.description("This is a test program");
#define DEFINE_PARAM(typ, par) \
   for (int i=0; i<ndelims; ++i) { \
     const std::string& pname=std::string(#par)+boost::lexical_cast<std::string>(i); \
     p.define< std::vector<typ> >(pname.c_str(),"No description"); \
   }

   DEFINE_PARAM(int,intvec_param);
   DEFINE_PARAM(double,dblvec_param);
   DEFINE_PARAM(bool,boolvec_param);
   
#undef DEFINE_PARAM   

   //verify correctness, for each delimiter
   for (int i=0; i<ndelims; ++i) {
#define COMPARE_PARAM(typ,par) \
     const std::vector<typ>& par ## _rd =p[std::string(#par)+boost::lexical_cast<std::string>(i)]; \
     EXPECT_EQ(par, par ## _rd)

     COMPARE_PARAM(int, intvec_param);
     COMPARE_PARAM(double, dblvec_param);
     COMPARE_PARAM(bool, boolvec_param);
#undef COMPARE_PARAM
   }
}

// Repeating parameters in the INI file
TEST(param,Repeating) {
    //create a file name
    std::string pfilename(alps::temporary_filename("pfile"));

   // Generate INI file
   {
     std::ofstream pfile(pfilename.c_str());
     pfile <<
         "parname = 1\n"
         "parname = 2\n";
   }

   // Imitate the command line args
   const int argc=2;
   const char* argv[2]={"THIS_PROGRAM",0};
   argv[1]=pfilename.c_str();

   //define the parameters
   alps::params p(argc,argv);
   p.description("This is a test program").
       define<int>("parname","repeating parameter");

   int n=0;
   EXPECT_THROW((n=p["parname"]),boost::program_options::multiple_occurrences);
}

// Unknown parameters in the INI file
TEST(param,Unknown) {
    //create a file name
    std::string pfilename(alps::temporary_filename("pfile"));

   // Generate INI file
   {
     std::ofstream pfile(pfilename.c_str());
     pfile <<
         "unknown = 2\n"
         "known = 1\n";
   }

   // Imitate the command line args
   const int argc=2;
   const char* argv[2]={"THIS_PROGRAM",0};
   argv[1]=pfilename.c_str();

   //define the parameters
   alps::params p(argc,argv);
   p.description("This is a test program").
       define<int>("known","known parameter");

   EXPECT_EQ((p["known"]),1);
}

// Trigger options in the command line
TEST(param,Triggers)
{
    const char* argv[]={ "", "--trigger_opt" };
    int argc=sizeof(argv)/sizeof(*argv);

    alps::params p(argc, argv);
    p.description("Trigger options test").
        define("trigger_opt","Trigger option").
        define("trigger_opt2","Trigger option, missing");
    EXPECT_TRUE(bool(p["trigger_opt"]));
    EXPECT_FALSE(bool(p["trigger_opt2"]));
}

// Command-line options overriding file options
TEST(param,CmdlineOverride)
{
    //create a file name
    std::string pfilename(alps::temporary_filename("pfile"));
    
    // Generate INI file
    {
        std::ofstream pfile(pfilename.c_str());
        pfile <<
            "param1 = 111\n"
            "param2 = 222\n";
    }

    // Imitate the command line args
    const char* argv[]={"THIS_PROGRAM",         // argv[0]
                        pfilename.c_str(),      // filename is the 1st argument
                        "--param1=999",         // override param1
                        "--param3=333",         // one more parameter
                        "--trigger_opt" };      // a trigger option  
    const int argc=sizeof(argv)/sizeof(*argv);
    
    alps::params p(argc, argv);
    p.
        define<int>("param1","Parameter 1").
        define<int>("param2","Parameter 2").
        define<int>("param3","Parameter 3").
        define("trigger_opt","Trigger param");

    EXPECT_EQ(999,p["param1"]);
    EXPECT_EQ(222,p["param2"]);
    EXPECT_EQ(333,p["param3"]);
    EXPECT_TRUE(bool(p["trigger_opt"]));
}

int main(int argc, char **argv) 
{
   // Test();
   // return 0;
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}