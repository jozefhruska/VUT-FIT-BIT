//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author JOZEF HRUŠKA
 *
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//
class EmptyTree : public ::testing::Test {
  protected:
    BinaryTree *binaryTree;

  virtual void SetUp() {
		binaryTree  = new BinaryTree();
  }

  virtual void TearDown() {
    delete binaryTree;
  }
};

class NonEmptyTree : public ::testing::Test {
  protected:
    BinaryTree *binaryTree;

  virtual void SetUp() {
		binaryTree  = new BinaryTree();
    binaryTree->InsertNode(1);
    binaryTree->InsertNode(2);
    binaryTree->InsertNode(3);
  }

  virtual void TearDown() {
    delete binaryTree;
  }
};

class TreeAxioms : public ::testing::Test {
  protected:
    BinaryTree *binaryTree;
    int array[11] =  {1, 4, 5, 6, 8, 9, 11, 13, 15, 16, 18};

  virtual void SetUp() {
		binaryTree  = new BinaryTree();
    for(int node : array) binaryTree->InsertNode(array[node]);
  }

  virtual void TearDown() {
    delete binaryTree;
  }
};

TEST_F(EmptyTree, InsertNode) {
  std::pair<bool, BinaryTree::Node_t *> newPair = binaryTree->InsertNode(0);
  EXPECT_TRUE(newPair.first);
  EXPECT_TRUE(newPair.second != NULL);
}

TEST_F(EmptyTree, DeleteNode) {
  EXPECT_FALSE(binaryTree->DeleteNode(0));
}

TEST_F(EmptyTree, FindNode) {
  EXPECT_TRUE(binaryTree->FindNode(0) == NULL);
}

TEST_F(NonEmptyTree, InsertNode) {
  std::pair<bool, BinaryTree::Node_t *> firstPair = binaryTree->InsertNode(0);
  EXPECT_TRUE(firstPair.first);
  EXPECT_TRUE(firstPair.second != NULL);

  std::pair<bool, BinaryTree::Node_t *> secondPair = binaryTree->InsertNode(1);
  EXPECT_FALSE(secondPair.first);
  EXPECT_TRUE(secondPair.second != NULL);

  std::pair<bool, BinaryTree::Node_t *> thirdPair = binaryTree->InsertNode(1);
  EXPECT_FALSE(thirdPair.first);
  EXPECT_TRUE(thirdPair.second == secondPair.second);
}

TEST_F(NonEmptyTree, DeleteNode) {
  EXPECT_TRUE(binaryTree->DeleteNode(1));
}

TEST_F(NonEmptyTree, FindeNode) {
  std::pair<bool, BinaryTree::Node_t *> newPair = binaryTree->InsertNode(0);
  BinaryTree::Node_t * node = binaryTree->FindNode(0);

  EXPECT_EQ(newPair.second, node);
}

TEST_F(TreeAxioms, Axiom1) {
  std::vector<BinaryTree::Node_t *> leafNodes;
  binaryTree->GetLeafNodes(leafNodes);

  for(BinaryTree::Node_t *node : leafNodes) {
    EXPECT_EQ(node->color, BinaryTree::BLACK);
  }
}

TEST_F(TreeAxioms, Axiom2) {
  std::vector<BinaryTree::Node_t *> allNodes;
  binaryTree->GetAllNodes(allNodes);

  for(BinaryTree::Node_t *node : allNodes) {
    if(node->color == BinaryTree::RED) {
      if(node->pLeft != NULL) EXPECT_EQ(node->pLeft->color, BinaryTree::BLACK);
      if(node->pRight != NULL) EXPECT_EQ(node->pRight->color, BinaryTree::BLACK);
    }
  }
}

TEST_F(TreeAxioms, Axiom3) {
  std::vector<BinaryTree::Node_t *> leafNodes;
  binaryTree->GetLeafNodes(leafNodes);

  int blackNodesArray[2] = {0, 0};
  bool firstRun = true;

  for (BinaryTree::Node_t *node : leafNodes) {
    int blackNodesCount = 0;
    BinaryTree::Node_t *iNode = node;

    while (iNode != NULL) {
      if (node->color == BinaryTree::BLACK) blackNodesCount++;
      iNode = iNode->pParent;
    }

    if (firstRun) EXPECT_EQ(blackNodesArray[0], blackNodesArray[1]);
    else firstRun = false;
    blackNodesArray[0] = blackNodesArray[1];
  }
}
/*** Konec souboru black_box_tests.cpp ***/
