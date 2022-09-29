#ifndef AST_H
#define AST_H

#include "lib/format/types.h"
#include "lib/string/types.h"
#include "lib/vector/vector.h"

#include "common.h"

#include "src/array/array.h"

typedef enum Ast_Type {
	Ast_Type_None = 0,
	Ast_Type_Array_Ptr,
	Ast_Type_Monadic,
	Ast_Type_Dyadic,
	Ast_Type_Assignment,
	Ast_Type_Identifier,
} Ast_Type;

typedef Array *                                   Array_Ptr;
typedef struct { Node_Handle func, right; }       Monadic;
typedef struct { Node_Handle func, left, right; } Dyadic;
typedef struct { Node_Handle left, right; }       Assignment;
typedef Short_String                              Identifier;

struct Ast_Node {
	Ast_Type type;
	union {
		Array_Ptr    as_Array_Ptr;
		Monadic      as_Monadic;
		Dyadic       as_Dyadic;
		Assignment   as_Assignment;
		Identifier   as_Identifier;
	};
};

typedef struct {
	Vector nodes;
	//Ast_Node *nodes;
	//u64 count;
	//u64 cap;
	Node_Handle parent;
} Ast;

void delete_ast(Ast *ast);
void flush_ast(Ast *ast);

// variant:    a possible type in struct `Ast_Node`'s anonymous union.
// bind_name:  an identifier
// expression: a value of type `Ast_Node`
//
// introduces a case statement for the corresponding `Ast_Type` value,
// and declares a variable `bind_name` initialized to the specified
// union value of `expression`
#define MATCH_AST(variant, bind_name, expression) MATCH_TYPED_UNION(Ast_Type_, variant, bind_name, expression, type)

// variant:    a possible type in struct `Ast_Node`'s anonymous union.
// bind_name:  an identifier
// expression: a value of type `Ast_Node`
//
// introduces a case statement for the corresponding `Ast_Type` value,
// and declares a variable `bind_name` initialized to a pointer to the
// specified union value of `expression`
#define REF_MATCH_AST(variant, bind_name, expression) REF_MATCH_TYPED_UNION(Ast_Type_, bind_name, variant, expression, type)

#endif // AST_H
