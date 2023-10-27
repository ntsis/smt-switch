/*********************                                                        */
/*! \file test-str.cpp
** \verbatim
** Top contributors (to current version):
**  Nestan Tsiskaridze
** This file is part of the smt-switch project.
** Copyright (c) 2020 by the authors listed in the file AUTHORS
** in the top-level source directory) and their institutional affiliations.
** All rights reserved.  See the file LICENSE in the top-level source
** directory for licensing information.\endverbatim
**
** \brief Tests for constants in theory of strings.
**
**
**/

#include <utility>
#include <vector>

// use this line for printing wstrings
//#include <locale>

#include "available_solvers.h"
#include "gtest/gtest.h"
#include "smt.h"


using namespace smt;
using namespace std;

namespace smt_tests {

GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(StrTests);
class StrTests : public ::testing::Test,
                 public ::testing::WithParamInterface<SolverConfiguration>
{
 protected:
  void SetUp() override
  {
    s = create_solver(GetParam());
    s->set_opt("produce-models", "true");
    s->set_opt("incremental", "true");
  }
  SmtSolver s;
};

TEST_P(StrTests, EqualVars)
{
  Sort str_sort = s->make_sort(STRING);
  Term x1 = s->make_symbol("x1", str_sort);
  Term x2 = s->make_symbol("x2", str_sort);
  s->assert_formula(s->make_term(Equal, x1, x2));
  s->check_sat();
  std::string strx1 = s->get_value(x1)->to_string();
  std::string strx2 = s->get_value(x2)->to_string();
  ASSERT_EQ(strx1, strx2);
}

TEST_P(StrTests, EqualStrConsts)
{
  Sort str_sort = s->make_sort(STRING);
  Term x1 = s->make_term("A", false, str_sort);
  Term x2 = s->make_term("A", false, str_sort);
  s->check_sat();
  std::string strx1 = s->get_value(x1)->to_string();
  std::string strx2 = s->get_value(x2)->to_string();
  ASSERT_EQ(strx1, strx2);
}


TEST_P(StrTests, UseEscSequences)
{
  // use these two lines for printing wstrings
  //std::locale::global(std::locale(""));
  //std::wcout.imbue(std::locale());

  Sort str_sort = s->make_sort(STRING);

  Term x1 = s->make_term("\\u{0021}", true, str_sort);
  Term x2 = s->make_term("\\u2200", true, str_sort);
  Term x3 = s->make_term("\\u2102", false, str_sort);
  Term x4 = s->make_term("\\u{10}", false, str_sort);

  s->check_sat();

  std:wstring wchar_u = L"u";
  std::wstring wstrx1 = s->get_value(x1)->getStringValue();
  std::wstring wstrx2 = s->get_value(x2)->getStringValue();
  std::wstring wstrx3 = s->get_value(x3)->getStringValue();
  std::wstring wstrx4 = s->get_value(x4)->getStringValue();


  // an example of printing wstrx1
  //std::cout << x1 << " = "; 
  //std::wcout << wstrx1 << std::endl;

  assert(wstrx1.find(wchar_u) == std::wstring::npos);
  assert(wstrx2.find(wchar_u) == std::wstring::npos);
  assert(wstrx3.find(wchar_u) != std::wstring::npos);
  assert(wstrx4.find(wchar_u) != std::wstring::npos);
}




TEST_P(StrTests, EqualVarStrVals)
{
  Sort str_sort = s->make_sort(STRING);
  Term x1 = s->make_symbol("x1", str_sort);
  Term x2 = s->make_symbol("x2", str_sort);
  Term A = s->make_term("A", false, str_sort);
  s->assert_formula(s->make_term(Equal, x1, A));
  s->assert_formula(s->make_term(Equal, x2, A));
  s->check_sat();
  std::string strx1 = s->get_value(x1)->to_string();
  std::string strx2 = s->get_value(x2)->to_string();
  ASSERT_EQ(strx1, strx2);
}

TEST_P(StrTests, EqualWStrConsts)
{
  Sort str_sort = s->make_sort(STRING);
  Term x1 = s->make_term(L"A", str_sort);
  Term x2 = s->make_term(L"A", str_sort);
  s->check_sat();
  std::string strx1 = s->get_value(x1)->to_string();
  std::string strx2 = s->get_value(x2)->to_string();
  ASSERT_EQ(strx1, strx2);
}

TEST_P(StrTests, EqualVarWStrVals)
{
  Sort str_sort = s->make_sort(STRING);
  Term x1 = s->make_symbol("x1", str_sort);
  Term x2 = s->make_symbol("x2", str_sort);
  Term A = s->make_term(L"A", str_sort);
  s->assert_formula(s->make_term(Equal, x1, A));
  s->assert_formula(s->make_term(Equal, x2, A));
  s->check_sat();
  std::string strx1 = s->get_value(x1)->to_string();
  std::string strx2 = s->get_value(x2)->to_string();
  ASSERT_EQ(strx1, strx2);
}

INSTANTIATE_TEST_SUITE_P(
    ParameterizedSolverStrTests,
    StrTests,
    testing::ValuesIn(filter_solver_configurations({ THEORY_INT, THEORY_STR })));

}  // namespace smt_tests