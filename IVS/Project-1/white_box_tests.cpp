//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     JOZEF HRUŠKA <xhrusk25@stud.fit.vutbr.cz>
// $Date:       $2018-03-05
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author JOZEF HRUŠKA
 *
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech
//    matic.
//============================================================================//
class MatrixTest : public ::testing::Test {};

class NonEmptyMatrixTest : public ::testing::Test {
  protected:
    Matrix *matrix;

  virtual void SetUp() {
    matrix = new Matrix(3, 3);

    matrix->set(0, 0, 00);
    matrix->set(0, 1, 01);
    matrix->set(0, 2, 02);

    matrix->set(1, 0, 10);
    matrix->set(1, 1, 11);
    matrix->set(1, 2, 12);

    matrix->set(2, 0, 20);
    matrix->set(2, 1, 21);
    matrix->set(2, 2, 22);
  }
};

TEST_F(MatrixTest, ConstructBasic) {
  EXPECT_NO_THROW(new Matrix());
  EXPECT_NO_THROW(new Matrix(1, 1));

  EXPECT_ANY_THROW(new Matrix(0, 0));
  EXPECT_ANY_THROW(new Matrix(0, 1));
  EXPECT_ANY_THROW(new Matrix(1, 0));
}

TEST_F(NonEmptyMatrixTest, GetValue) {
  EXPECT_EQ(matrix->get(0, 1), 01);
  EXPECT_EQ(matrix->get(2, 2), 22);

  EXPECT_ANY_THROW(matrix->get(0, 4));
}

TEST_F(NonEmptyMatrixTest, SetValue) {
  matrix->set(0, 1, 5);
  EXPECT_EQ(matrix->get(0, 1), 5);

  EXPECT_FALSE(matrix->set(0, 4, 04));
}

TEST_F(NonEmptyMatrixTest, SetMultipleValues) {
  std::vector<std::vector<double>> vector = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
  };

  EXPECT_TRUE(matrix->set(vector));
}

TEST_F(NonEmptyMatrixTest, SetMultipleValuesBadSize) {
  std::vector<std::vector<double>> vectorA = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
  };
  matrix->set(vectorA);

  std::vector<std::vector<double>> vectorB = {
    {1, 2},
    {4, 5},
    {7, 9}
  };

  EXPECT_FALSE(matrix->set(vectorB));
}

TEST_F(MatrixTest, OperatorEquals) {
  Matrix matrixA(3, 3), matrixB(3, 3), matrixC(3, 4);

  std::vector<std::vector<double>> vectorA = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
  };

  std::vector<std::vector<double>> vectorC = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12}
  };

  matrixA.set(vectorA);
  matrixB.set(vectorA);
  EXPECT_TRUE(matrixA == matrixB);

  matrixB.set(0, 0, 100);
  EXPECT_FALSE(matrixA == matrixB);

  matrixC.set(vectorC);
  EXPECT_ANY_THROW(matrixA == matrixC);
}

TEST_F(MatrixTest, OperatorAdd) {
  Matrix matrixA(3, 3), matrixB(3, 3), matrixC(3, 4);

  std::vector<std::vector<double>> vectorA = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
  };

  std::vector<std::vector<double>> vectorARes = {
    {2, 4, 6},
    {8, 10, 12},
    {14, 16, 18}
  };

  std::vector<std::vector<double>> vectorC = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12}
  };

  matrixA.set(vectorA);
  matrixB.set(vectorARes);
  EXPECT_TRUE(matrixB == (matrixA + matrixA));

  matrixA.set(vectorA);
  matrixC.set(vectorC);
  EXPECT_ANY_THROW(matrixA + matrixC);
}

TEST_F(MatrixTest, EquationBadMatrix) {
  Matrix matrixA(2, 2), matrixB(3, 3), matrixC(3, 4), matrixD(3, 2);

  std::vector<std::vector<double>> vectorA = {
			{1, -2},
			{-2, 4},
	};
  matrixA.set(vectorA);
  EXPECT_ANY_THROW(matrixA.solveEquation(std::vector<double> {2, 2}));

  std::vector<std::vector<double>> vectorB = {
    {1, 1, 2},
    {4, 5, 6},
    {7, 8, 9}
  };
  matrixB.set(vectorB);
  EXPECT_ANY_THROW(matrixB.solveEquation(std::vector<double> {}));
  EXPECT_ANY_THROW(matrixB.solveEquation(std::vector<double> {1, 1}));

  std::vector<std::vector<double>> vectorC = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12}
  };
  matrixC.set(vectorC);
  EXPECT_ANY_THROW(matrixC.solveEquation(std::vector<double> {10, 20, 30}));

  std::vector<std::vector<double>> vectorD = {
    {1, 1},
    {4, 5},
    {7, 8}
  };
  matrixD.set(vectorD);
  EXPECT_ANY_THROW(matrixD.solveEquation(std::vector<double> {10, 20, 30}));
}

TEST_F(MatrixTest, EquationSolve) {
  Matrix matrixA(2, 2), matrixB(3, 3), matrixC(3, 4), matrixD(3, 2);

  std::vector<std::vector<double>> vectorA = {
			{2, -3},
			{1, -2}
	};
  std::vector<double> vectorARes = {
    {7, 3}
  };
  matrixA.set(vectorA);
  EXPECT_TRUE(matrixA.solveEquation(std::vector<double> {5, 1}) == vectorARes);

  std::vector<std::vector<double>> vectorB = {
    {3, 1, 1},
    {2, 2, 5},
    {1, -3, -4}
  };
  std::vector<double> vectorBRes = {
    {1, 1, -1}
  };
  matrixB.set(vectorB);
  EXPECT_TRUE(matrixB.solveEquation(std::vector<double> {3, -1, 2}) == vectorBRes);
}

/*** Konec souboru white_box_tests.cpp ***/
