/* Copyright (c) 2020 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#include "exec/logic/SelectExecutor.h"

#include "planner/Query.h"
#include "context/ExpressionContextImpl.h"

namespace nebula {
namespace graph {

SelectExecutor::SelectExecutor(const PlanNode* node,
                               QueryContext* qctx,
                               Executor* then,
                               Executor* els)
    : Executor("SelectExecutor", node, qctx),
      then_(DCHECK_NOTNULL(then)),
      else_(DCHECK_NOTNULL(els)) {}

folly::Future<Status> SelectExecutor::execute() {
    dumpLog();

    auto* select = asNode<Select>(node());
    auto* expr = select->condition();
    ExpressionContextImpl ctx(ectx_, nullptr);
    auto value = expr->eval(ctx);
    DCHECK(value.isBool());
    finish(std::move(value));
    return Status::OK();
}

}   // namespace graph
}   // namespace nebula