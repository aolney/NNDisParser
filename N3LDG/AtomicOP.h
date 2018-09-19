#ifndef ATOMICIOP_H_
#define ATOMICIOP_H_

/*
*  AtomicOP.h:
*  a list of atomic operations
*
*  Created on: June 11, 2017
*      Author: yue_zhang(suda), mszhang
*/

/*
ActivateNode
TanhNode
SigmoidNode
ReluNode
IndexNode
PSubNode
PDotNode
*/

#include "Param.h"
#include "MyLib.h"
#include "Node.h"
#include "Graph.h"
#include "ModelUpdate.h"

class ActivateNode :public Node {
  public:
    PNode in;
    dtype(*activate)(const dtype&);   // activation function
    dtype(*derivate)(const dtype&, const dtype&);  // derivation function of activation function

  public:
    ActivateNode() : Node() {
        in = NULL;
        activate = ftanh;
        derivate = dtanh;
        node_type = "activate";
    }

    ~ActivateNode() {
        in = NULL;
    }

    inline void clearValue() {
        Node::clearValue();
        in = NULL;
    }

    // define the activate function and its derivation form
    inline void setFunctions(dtype(*f)(const dtype&), dtype(*f_deri)(const dtype&, const dtype&)) {
        activate = f;
        derivate = f_deri;
    }

  public:
    void forward(Graph *cg, PNode x) {
        in = x;
        degree = 0;
        in->addParent(this);
        cg->addNode(this);
    }

  public:
    inline void compute() {
        val.vec() = in->val.vec().unaryExpr(ptr_fun(activate));
    }

    void backward() {
        in->loss.vec() += loss.vec() * in->val.vec().binaryExpr(val.vec(), ptr_fun(derivate));
    }

  public:
    inline PExecute generate(bool bTrain, dtype cur_drop_factor);

    // better to rewrite for deep understanding
    inline bool typeEqual(PNode other) {
        bool result = Node::typeEqual(other);
        return result;
    }
};


class ActivateExecute :public Execute {
  public:
    bool bTrain;
  public:
    inline void  forward() {
        int count = batch.size();
        //#pragma omp parallel for
        for (int idx = 0; idx < count; idx++) {
            batch[idx]->compute();
            batch[idx]->forward_drop(bTrain, drop_factor);
        }
    }

    inline void backward() {
        int count = batch.size();
        //#pragma omp parallel for
        for (int idx = 0; idx < count; idx++) {
            batch[idx]->backward_drop();
            batch[idx]->backward();
        }
    }
};

inline PExecute ActivateNode::generate(bool bTrain, dtype cur_drop_factor) {
    ActivateExecute* exec = new ActivateExecute();
    exec->batch.push_back(this);
    exec->bTrain = bTrain;
    exec->drop_factor = cur_drop_factor;
    return exec;
};

class TanhNode :public Node {
  public:
    PNode in;

  public:
    TanhNode() : Node() {
        in = NULL;
        node_type = "tanh";
    }

    ~TanhNode() {
        in = NULL;
    }

    inline void clearValue() {
        Node::clearValue();
        in = NULL;
    }

  public:
    void forward(Graph *cg, PNode x) {
        in = x;
        degree = 0;
        in->addParent(this);
        cg->addNode(this);
    }

  public:
    inline void compute() {
        val.vec() = in->val.vec().unaryExpr(ptr_fun(ftanh));
    }

    void backward() {
        in->loss.vec() += loss.vec() * in->val.vec().binaryExpr(val.vec(), ptr_fun(dtanh));
    }

  public:
    inline PExecute generate(bool bTrain, dtype cur_drop_factor);

    // better to rewrite for deep understanding
    inline bool typeEqual(PNode other) {
        bool result = Node::typeEqual(other);
        return result;
    }
};

class TanhExecute :public Execute {
  public:
    bool bTrain;
  public:
    inline void  forward() {
        int count = batch.size();
        //#pragma omp parallel for
        for (int idx = 0; idx < count; idx++) {
            batch[idx]->compute();
            batch[idx]->forward_drop(bTrain, drop_factor);
        }
    }

    inline void backward() {
        int count = batch.size();
        //#pragma omp parallel for
        for (int idx = 0; idx < count; idx++) {
            batch[idx]->backward_drop();
            batch[idx]->backward();
        }
    }
};

inline PExecute TanhNode::generate(bool bTrain, dtype cur_drop_factor) {
    TanhExecute* exec = new TanhExecute();
    exec->batch.push_back(this);
    exec->bTrain = bTrain;
    exec->drop_factor = cur_drop_factor;
    return exec;
};


class SigmoidNode :public Node {
  public:
    PNode in;

  public:
    SigmoidNode() : Node() {
        in = NULL;
        node_type = "sigmoid";
    }

    ~SigmoidNode() {
        in = NULL;
    }

    inline void clearValue() {
        Node::clearValue();
        in = NULL;
    }

  public:
    void forward(Graph *cg, PNode x) {
        in = x;
        degree = 0;
        in->addParent(this);
        cg->addNode(this);
    }

  public:
    inline void compute() {
        val.vec() = in->val.vec().unaryExpr(ptr_fun(fsigmoid));
    }

    void backward() {
        in->loss.vec() += loss.vec() * in->val.vec().binaryExpr(val.vec(), ptr_fun(dsigmoid));
    }

  public:
    inline PExecute generate(bool bTrain, dtype cur_drop_factor);

    // better to rewrite for deep understanding
    inline bool typeEqual(PNode other) {
        bool result = Node::typeEqual(other);
        return result;
    }
};

class SigmoidExecute :public Execute {
  public:
    bool bTrain;
  public:
    inline void  forward() {
        int count = batch.size();
        //#pragma omp parallel for
        for (int idx = 0; idx < count; idx++) {
            batch[idx]->compute();
            batch[idx]->forward_drop(bTrain, drop_factor);
        }
    }

    inline void backward() {
        int count = batch.size();
        //#pragma omp parallel for
        for (int idx = 0; idx < count; idx++) {
            batch[idx]->backward_drop();
            batch[idx]->backward();
        }
    }
};

inline PExecute SigmoidNode::generate(bool bTrain, dtype cur_drop_factor) {
    SigmoidExecute* exec = new SigmoidExecute();
    exec->batch.push_back(this);
    exec->bTrain = bTrain;
    exec->drop_factor = cur_drop_factor;
    return exec;
};


class ReluNode :public Node {
  public:
    PNode in;

  public:
    ReluNode() : Node() {
        in = NULL;
        node_type = "relu";
    }

    ~ReluNode() {
        in = NULL;
    }

    inline void clearValue() {
        Node::clearValue();
        in = NULL;
    }


  public:
    void forward(Graph *cg, PNode x) {
        in = x;
        degree = 0;
        in->addParent(this);
        cg->addNode(this);
    }

  public:
    inline void compute() {
        val.vec() = in->val.vec().unaryExpr(ptr_fun(frelu));
    }

    void backward() {
        in->loss.vec() += loss.vec() * in->val.vec().binaryExpr(val.vec(), ptr_fun(drelu));
    }

  public:
    inline PExecute generate(bool bTrain, dtype cur_drop_factor);

    // better to rewrite for deep understanding
    inline bool typeEqual(PNode other) {
        bool result = Node::typeEqual(other);
        return result;
    }
};

class ReluExecute :public Execute {
  public:
    bool bTrain;
  public:
    inline void  forward() {
        int count = batch.size();
        //#pragma omp parallel for
        for (int idx = 0; idx < count; idx++) {
            batch[idx]->compute();
            batch[idx]->forward_drop(bTrain, drop_factor);
        }
    }

    inline void backward() {
        int count = batch.size();
        //#pragma omp parallel for
        for (int idx = 0; idx < count; idx++) {
            batch[idx]->backward_drop();
            batch[idx]->backward();
        }
    }
};

inline PExecute ReluNode::generate(bool bTrain, dtype cur_drop_factor) {
    ReluExecute* exec = new ReluExecute();
    exec->batch.push_back(this);
    exec->bTrain = bTrain;
    exec->drop_factor = cur_drop_factor;
    return exec;
};


class IndexNode :public Node {
  public:
    PNode in;
    int index_id;

  public:
    IndexNode() : Node() {
        in = NULL;
        index_id = -1;
        dim = 1;
        node_type = "index";
    }

    ~IndexNode() {
        in = NULL;
    }

    inline void clearValue() {
        Node::clearValue();
        in = NULL;
        index_id = -1;
    }

    //can not be dropped since the output is a scalar
    inline void init(int ndim, dtype dropout) {
        dim = 1;
        Node::init(dim, -1);
    }

  public:
    void forward(Graph *cg, PNode x, int index) {
        in = x;
        index_id = index;
        degree = 0;
        in->addParent(this);
        cg->addNode(this);
    }

  public:
    void compute() {
        val[0] = in->val[index_id];
    }

    void backward() {
        in->loss[index_id] += loss[0];
    }

  public:
    inline PExecute generate(bool bTrain, dtype cur_drop_factor);

    // better to rewrite for deep understanding
    inline bool typeEqual(PNode other) {
        bool result = Node::typeEqual(other);
        return result;
    }
};

class IndexExecute : public Execute {
  public:
    bool bTrain;
  public:
    inline void  forward() {
        int count = batch.size();
        //#pragma omp parallel for
        for (int idx = 0; idx < count; idx++) {
            batch[idx]->compute();
            batch[idx]->forward_drop(bTrain, drop_factor);
        }
    }

    inline void backward() {
        int count = batch.size();
        //#pragma omp parallel for
        for (int idx = 0; idx < count; idx++) {
            batch[idx]->backward_drop();
            batch[idx]->backward();
        }
    }
};

inline PExecute IndexNode::generate(bool bTrain, dtype cur_drop_factor) {
    IndexExecute* exec = new IndexExecute();
    exec->batch.push_back(this);
    exec->bTrain = bTrain;
    exec->drop_factor = cur_drop_factor;
    return exec;
}



class PSubNode : public Node {
  public:
    PNode in1, in2;
  public:
    PSubNode() : Node() {
        in1 = NULL;
        in2 = NULL;
        node_type = "point-subtraction";
    }
  public:
    virtual inline void clearValue() {
        Node::clearValue();
        in1 = NULL;
        in2 = NULL;
    }


  public:
    void forward(Graph *cg, PNode x1, PNode x2) {
        in1 = x1;
        in2 = x2;
        degree = 0;
        in1->addParent(this);
        in2->addParent(this);
        cg->addNode(this);
    }

  public:
    inline void compute() {
        val.vec() = in1->val.vec() - in2->val.vec();
    }

    void backward() {
        in1->loss.vec() += loss.vec();
        in2->loss.vec() -= loss.vec();
    }

  public:
    // better to rewrite for deep understanding
    inline bool typeEqual(PNode other) {
        return Node::typeEqual(other);
    }

    inline PExecute generate(bool bTrain, dtype cur_drop_factor);
};


class PSubExecute :public Execute {
  public:
    bool bTrain;
  public:
    inline void  forward() {
        int count = batch.size();
        //#pragma omp parallel for
        for (int idx = 0; idx < count; idx++) {
            batch[idx]->compute();
            batch[idx]->forward_drop(bTrain, drop_factor);
        }
    }

    inline void backward() {
        int count = batch.size();
        //#pragma omp parallel for
        for (int idx = 0; idx < count; idx++) {
            batch[idx]->backward_drop();
            batch[idx]->backward();
        }
    }
};

inline PExecute PSubNode::generate(bool bTrain, dtype cur_drop_factor) {
    PSubExecute* exec = new PSubExecute();
    exec->batch.push_back(this);
    exec->bTrain = bTrain;
    exec->drop_factor = cur_drop_factor;
    return exec;
}


class PDotNode : public Node {
  public:
    PNode in1, in2;
  public:
    PDotNode() : Node() {
        in1 = NULL;
        in2 = NULL;
        dim = 1;
        node_type = "point-dot";
    }
  public:
    virtual inline void clearValue() {
        Node::clearValue();
        in1 = NULL;
        in2 = NULL;
    }

    //can not be dropped since the output is a scalar
    inline void init(int ndim, dtype dropout) {
        dim = 1;
        Node::init(dim, -1);
    }

  public:
    void forward(Graph *cg, PNode x1, PNode x2) {
        in1 = x1;
        in2 = x2;
        degree = 0;
        in1->addParent(this);
        in2->addParent(this);
        cg->addNode(this);
    }

  public:
    inline void compute() {
        val[0] = 0.0;
        for (int idx = 0; idx < in1->dim; idx++) {
            val[0] += in1->val[idx] * in2->val[idx];
        }
    }

    void backward() {
        for (int idx = 0; idx < in1->dim; idx++) {
            in1->loss[idx] += loss[0] * in2->val[idx];
            in2->loss[idx] += loss[0] * in1->val[idx];
        }
    }

  public:
    // better to rewrite for deep understanding
    inline bool typeEqual(PNode other) {
        return Node::typeEqual(other);
    }

    inline PExecute generate(bool bTrain, dtype cur_drop_factor);
};

class PDotExecute :public Execute {
  public:
    bool bTrain;
  public:
    inline void  forward() {
        int count = batch.size();
        //#pragma omp parallel for
        for (int idx = 0; idx < count; idx++) {
            batch[idx]->compute();
            batch[idx]->forward_drop(bTrain, drop_factor);
        }
    }

    inline void backward() {
        int count = batch.size();
        //#pragma omp parallel for
        for (int idx = 0; idx < count; idx++) {
            batch[idx]->backward_drop();
            batch[idx]->backward();
        }
    }
};


inline PExecute PDotNode::generate(bool bTrain, dtype cur_drop_factor) {
    PDotExecute* exec = new PDotExecute();
    exec->batch.push_back(this);
    exec->bTrain = bTrain;
    exec->drop_factor = cur_drop_factor;
    return exec;
}

#endif
