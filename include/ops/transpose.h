#ifndef _FLEXFLOW_TRANSPOSE_H_
#define _FLEXFLOW_TRANSPOSE_H_

#include "model.h"

class TransposeMeta : public OpMeta {
public:
  TransposeMeta(FFHandler handler) : OpMeta(handler) {};
  int num_dim;
  int perm[MAX_TENSOR_DIM];
};

class Transpose : public Op {
public:
  Transpose(FFModel& model,
            const Tensor input,
            const std::vector<int>& perm,
            const char* name);
  void init(const FFModel&);
  void forward(const FFModel&);
  void backward(const FFModel&);
  void print_layer(const FFModel& model) {assert(0);}

  static OpMeta* init_task(const Legion::Task *task,
                           const std::vector<Legion::PhysicalRegion> &regions,
                           Legion::Context ctx, Legion::Runtime *runtime);
  void init_meta(TransposeMeta *m,
                 Legion::Domain const &in_domain,
                 Legion::Domain const &out_domain) const;
  static void forward_task(const Legion::Task *task,
                           const std::vector<Legion::PhysicalRegion> &regions,
                           Legion::Context ctx, Legion::Runtime *runtime);
  static void backward_task(const Legion::Task *task,
                            const std::vector<Legion::PhysicalRegion> &regions,
                            Legion::Context ctx, Legion::Runtime *runtime);
  static void forward_kernel(const TransposeMeta* m,
                             const float* input_ptr,
                             float* output_ptr,
                             Legion::Domain in_domain,
                             Legion::Domain out_domain,
			     cudaStream_t stream);
  static void backward_kernel(const TransposeMeta* m,
                              float* input_grad_ptr,
                              const float* output_grad_ptr,
                              Legion::Domain in_grad_domain,
                              Legion::Domain out_grad_domain,
			      cudaStream_t stream);
  bool measure_operator_cost(Simulator* sim,
                             const ParallelConfig& pc,
                             CostMetrics& cost_metrics) const;
public:
  int perm[MAX_TENSOR_DIM];
};

#endif