/*++
Copyright (c) 2012 Microsoft Corporation

Module Name:

    cmd_context_to_goal.cpp

Abstract:
    Procedure for copying the assertions in the
    command context to a goal object.

Author:

    Leonardo (leonardo) 2012-10-21

Notes:

--*/
#include"cmd_context.h"
#include"goal.h"

/**
   \brief Assert expressions from ctx into t.
*/
void assert_exprs_from(cmd_context const & ctx, goal & t) {
    if (ctx.produce_proofs() && ctx.produce_unsat_cores()) 
        throw cmd_exception("Frontend does not support simultaneous generation of proofs and unsat cores");
    ast_manager & m = t.m();
    bool proofs_enabled = t.proofs_enabled();
    ptr_vector<expr>::const_iterator it  = ctx.begin_assertions();
    ptr_vector<expr>::const_iterator end = ctx.end_assertions();
    for (; it != end; ++it) {
        t.assert_expr(*it, proofs_enabled ? m.mk_asserted(*it) : 0, 0);
    }
    if (ctx.produce_unsat_cores()) {
        SASSERT(!ctx.produce_proofs());
        it  = ctx.begin_assumptions();
        end = ctx.end_assumptions();
        for (; it != end; ++it) {
            t.assert_expr(*it, 0, m.mk_leaf(*it));
        }
    }
    else {
        SASSERT(ctx.begin_assumptions() == ctx.end_assumptions());
    }
}
