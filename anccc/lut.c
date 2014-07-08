/*
    lut.c: look up table
    Aean, 2014.7.6
*/

#include "anccc.h"

#define K(v) { v, #v }
symbol_t* symbol_list[] = {terminal_list, nonterminal_list};
symbol_t terminal_list[] = {
    K(IDENT),K(C_CHAR),K(C_STRING),K(C_SINGLE),K(C_DOUBLE),K(C_SIGNED),K(C_UNSIGNED),K(C_SIGNEDLONG),K(C_UNSIGNEDLONG),
    K(C_SIGNEDLONGLONG),K(C_UNSIGNEDLONGLONG),K(K_AUTO),K(K_BREAK),K(K_CASE),K(K_CHAR),K(K_CONST),K(K_CONTINUE),
    K(K_DEFAULT),K(K_DO),K(K_DOUBLE),K(K_ELSE),K(K_ENUM),K(K_EXTERN),K(K_FLOAT),K(K_FOR),K(K_GOTO),K(K_IF),K(K_INLINE),
    K(K_INT),K(K_LONG),K(K_REGISTER),K(K_RESTRICT),K(K_RETURN),K(K_SHORT),K(K_SIGNED),K(K_SIZEOF),K(K_STATIC),K(K_STRUCT),
    K(K_SWITCH),K(K_TYPEDEF),K(K_UNION),K(K_UNSIGNED),K(K_VOID),K(K_VOLATILE),K(K_WHILE),K(K_BOOL),K(K_COMPLEX),
    K(K_IMAGINARY),K(PUNC1),K(PUNC2),K(PUNC3),K(PUNC4),K(PUNC5),K(PUNC6),K(PUNC7),K(PUNC8),K(PUNC9),K(PUNC10),K(PUNC11),
    K(PUNC12),K(PUNC13),K(PUNC14),K(PUNC15),K(PUNC16),K(PUNC17),K(PUNC18),K(PUNC19),K(PUNC20),K(PUNC21),K(PUNC22),
    K(PUNC23),K(PUNC24),K(PUNC25),K(PUNC26),K(PUNC27),K(PUNC28),K(PUNC29),K(PUNC30),K(PUNC31),K(PUNC32),K(PUNC33),
    K(PUNC34),K(PUNC35),K(PUNC36),K(PUNC37),K(PUNC38),K(PUNC39),K(PUNC40),K(PUNC41),K(PUNC42),K(PUNC43),K(PUNC44),
    K(PUNC45),K(PUNC46),K(PUNC47),K(PUNC48),K(NAL),
    {EOL, NULL}
};
symbol_t nonterminal_list[] = {
    K(string_literal),
    K(primary_expression),
    K(postfix_expression),
    K(argument_expression_list),
    K(unary_expression),
    K(unary_operator),
    K(cast_expression),
    K(multiplicative_expression),
    K(additive_expression),
    K(shift_expression),
    K(relational_expression),
    K(equality_expression),
    K(AND_expression),
    K(exclusive_OR_expression),
    K(inclusive_OR_expression),
    K(logical_AND_expression),
    K(logical_OR_expression),
    K(conditional_expression),
    K(assignment_expression),
    K(assignment_operator),
    K(expression),
    K(constant_expression),
    K(declaration),
    K(declaration_specifiers),
    K(init_declarator_list),
    K(init_declarator),
    K(storage_class_specifier),
    K(type_specifier),
    K(struct_or_union_specifier),
    K(struct_or_union),
    K(struct_declaration_list),
    K(struct_declaration),
    K(specifier_qualifier_list),
    K(struct_declarator_list),
    K(struct_declarator),
    K(enum_specifier),
    K(enumerator_list),
    K(enumerator),
    K(type_qualifier),
    K(function_specifier),
    K(declarator),
    K(direct_declarator),
    K(pointer),
    K(type_qualifier_list),
    K(parameter_type_list),
    K(parameter_list),
    K(parameter_declaration),
    K(identifier_list),
    K(type_name),
    K(abstract_declarator),
    K(direct_abstract_declarator),
    K(direct_abstract_declarator_final),
    K(typedef_name),
    K(initializer),
    K(initializer_list),
    K(designation),
    K(designator_list),
    K(designator),
    K(statement),
    K(labeled_statement),
    K(compound_statement),
    K(block_item_list),
    K(block_item),
    K(expression_statement),
    K(selection_statement),
    K(iteration_statement),
    K(jump_statement),
    K(translation_unit),
    K(external_declaration),
    K(function_definition),
    K(declaration_list),
    {EOL, NULL}
};

#define R(l,...) { l, {__VA_ARGS__, EOL} }
rule_t rule_list[] = {
    R(string_literal, C_STRING),
    R(string_literal, string_literal, C_STRING),
    R(constant, IDENT),
    R(constant, C_SINGLE),
    R(constant, C_DOUBLE),
    R(constant, C_SIGNED),
    R(constant, C_UNSIGNED),
    R(constant, C_SIGNEDLONG),
    R(constant, C_UNSIGNEDLONG),
    R(constant, C_SIGNEDLONGLONG),
    R(constant, C_UNSIGNEDLONGLONG),
    R(constant, C_CHAR),
    R(primary_expression, IDENT),
    R(primary_expression, constant),
    R(primary_expression, string_literal),
    R(primary_expression, PUNC3, expression, PUNC4),
    R(postfix_expression, primary_expression),
    R(postfix_expression, postfix_expression, PUNC1, expression, PUNC2),
    R(postfix_expression, postfix_expression, PUNC3, argument_expression_list, PUNC4),
    R(postfix_expression, postfix_expression, PUNC7, IDENT),
    R(postfix_expression, postfix_expression, PUNC8, IDENT),
    R(postfix_expression, postfix_expression, PUNC9),
    R(postfix_expression, postfix_expression, PUNC10),
    R(postfix_expression, PUNC3, type_name, PUNC4, PUNC5, initializer_list, PUNC6),
    R(postfix_expression, PUNC3, type_name, PUNC4, PUNC5, initializer_list, PUNC46, PUNC6),
    R(argument_expression_list, assignment_expression),
    R(argument_expression_list, argument_expression_list, PUNC46, assignment_expression),
    R(unary_expression, postfix_expression),
    R(unary_expression, PUNC9, unary_expression),
    R(unary_expression, PUNC10, unary_expression),
    R(unary_expression, unary_operator, cast_expression),
    R(unary_expression, K_SIZEOF, unary_expression),
    R(unary_expression, K_SIZEOF, PUNC3, type_name, PUNC4),
    R(unary_operator, PUNC11),
    R(unary_operator, PUNC12),
    R(unary_operator, PUNC13),
    R(unary_operator, PUNC14),
    R(unary_operator, PUNC15),
    R(unary_operator, PUNC16),
    R(cast_expression, unary_expression),
    R(cast_expression, PUNC3, type_name, PUNC4, cast_expression),
    R(multiplicative_expression, cast_expression),
    R(multiplicative_expression, multiplicative_expression, PUNC12, cast_expression),
    R(multiplicative_expression, multiplicative_expression, PUNC17, cast_expression),
    R(multiplicative_expression, multiplicative_expression, PUNC18, cast_expression),
    R(additive_expression, multiplicative_expression),
    R(additive_expression, additive_expression, PUNC13, multiplicative_expression),
    R(additive_expression, additive_expression, PUNC14, multiplicative_expression),
    R(shift_expression, additive_expression),
    R(shift_expression, shift_expression, PUNC19, additive_expression),
    R(shift_expression, shift_expression, PUNC20, additive_expression),
    R(relational_expression, shift_expression),
    R(relational_expression, relational_expression, PUNC21, shift_expression),
    R(relational_expression, relational_expression, PUNC22, shift_expression),
    R(relational_expression, relational_expression, PUNC23, shift_expression),
    R(relational_expression, relational_expression, PUNC24, shift_expression),
    R(equality_expression, relational_expression),
    R(equality_expression, equality_expression, PUNC25, relational_expression),
    R(equality_expression, equality_expression, PUNC26, relational_expression),
    R(AND_expression, equality_expression),
    R(AND_expression, AND_expression, PUNC11, equality_expression),
    R(exclusive_OR_expression, AND_expression),
    R(exclusive_OR_expression, exclusive_OR_expression, PUNC27, AND_expression),
    R(inclusive_OR_expression, exclusive_OR_expression),
    R(inclusive_OR_expression, inclusive_OR_expression, PUNC28, exclusive_OR_expression),
    R(logical_AND_expression, inclusive_OR_expression),
    R(logical_AND_expression, logical_AND_expression, PUNC29, inclusive_OR_expression),
    R(logical_OR_expression, logical_AND_expression),
    R(logical_OR_expression, logical_OR_expression, PUNC30, logical_AND_expression),
    R(conditional_expression, logical_OR_expression),
    R(conditional_expression, logical_OR_expression, PUNC31, expression, PUNC32, conditional_expression),
    R(assignment_expression, conditional_expression),
    R(assignment_expression, unary_expression, assignment_operator, assignment_expression),
    R(assignment_operator, PUNC35),
    R(assignment_operator, PUNC36),
    R(assignment_operator, PUNC37),
    R(assignment_operator, PUNC38),
    R(assignment_operator, PUNC39),
    R(assignment_operator, PUNC40),
    R(assignment_operator, PUNC41),
    R(assignment_operator, PUNC42),
    R(assignment_operator, PUNC43),
    R(assignment_operator, PUNC44),
    R(assignment_operator, PUNC45),
    R(expression, assignment_expression),
    R(expression, expression, PUNC46, assignment_expression),
    R(constant_expression, conditional_expression),
    R(declaration, declaration_specifiers, init_declarator_list, PUNC33),
    R(declaration, declaration_specifiers, PUNC33),
    R(declaration_specifiers, storage_class_specifier),
    R(declaration_specifiers, storage_class_specifier, declaration_specifiers),
    R(declaration_specifiers, type_specifier),
    R(declaration_specifiers, type_specifier, declaration_specifiers),
    R(declaration_specifiers, type_qualifier),
    R(declaration_specifiers, type_qualifier, declaration_specifiers),
    R(declaration_specifiers, function_specifier),
    R(declaration_specifiers, function_specifier, declaration_specifiers),
    R(init_declarator_list, init_declarator),
    R(init_declarator_list, init_declarator_list, PUNC46, init_declarator),
    R(init_declarator, declarator),
    R(init_declarator, declarator, PUNC35, initializer),
    R(storage_class_specifier, K_TYPEDEF),
    R(storage_class_specifier, K_EXTERN),
    R(storage_class_specifier, K_STATIC),
    R(storage_class_specifier, K_AUTO),
    R(storage_class_specifier, K_REGISTER),
    R(type_specifier, K_VOID),
    R(type_specifier, K_CHAR),
    R(type_specifier, K_SHORT),
    R(type_specifier, K_INT),
    R(type_specifier, K_LONG),
    R(type_specifier, K_FLOAT),
    R(type_specifier, K_DOUBLE),
    R(type_specifier, K_SIGNED),
    R(type_specifier, K_UNSIGNED),
    R(type_specifier, K_BOOL),
    R(type_specifier, K_COMPLEX),
    R(type_specifier, struct_or_union_specifier),
    R(type_specifier, enum_specifier),
    R(type_specifier, typedef_name),
    R(struct_or_union_specifier, struct_or_union, IDENT),
    R(struct_or_union_specifier, struct_or_union, IDENT, PUNC5, struct_declaration_list, PUNC6),
    R(struct_or_union_specifier, struct_or_union, PUNC5, struct_declaration_list, PUNC6),
    R(struct_or_union, K_STRUCT),
    R(struct_or_union, K_UNION),
    R(struct_declaration_list, struct_declaration),
    R(struct_declaration_list, struct_declaration_list, struct_declaration),
    R(struct_declaration, specifier_qualifier_list, struct_declarator_list, PUNC33),
    R(specifier_qualifier_list, type_specifier),
    R(specifier_qualifier_list, type_specifier, specifier_qualifier_list),
    R(specifier_qualifier_list, type_qualifier),
    R(specifier_qualifier_list, type_qualifier, specifier_qualifier_list),
    R(struct_declarator_list, struct_declarator),
    R(struct_declarator_list, struct_declarator_list, PUNC46, struct_declarator),
    R(struct_declarator, declarator),
    R(struct_declarator, declarator, PUNC32, constant_expression),
    R(struct_declarator, PUNC32, constant_expression),
    R(enum_specifier, K_ENUM, IDENT),
    R(enum_specifier, K_ENUM, IDENT, PUNC5, enumerator_list, PUNC6),
    R(enum_specifier, K_ENUM, IDENT, PUNC5, enumerator_list, PUNC46, PUNC6),
    R(enum_specifier, K_ENUM, PUNC5, enumerator_list, PUNC6),
    R(enum_specifier, K_ENUM, PUNC5, enumerator_list, PUNC46, PUNC6),
    R(enumerator_list, enumerator),
    R(enumerator_list, enumerator_list, PUNC46, enumerator),
    R(enumerator, IDENT),
    R(enumerator, IDENT, PUNC35, constant_expression),
    R(type_qualifier, K_CONST),
    R(type_qualifier, K_RESTRICT),
    R(type_qualifier, K_VOLATILE),
    R(function_specifier, K_INLINE),
    R(declarator, pointer, direct_declarator),
    R(declarator, direct_declarator),
    R(direct_declarator, IDENT),
    R(direct_declarator, PUNC3, declarator, PUNC4),
    R(direct_declarator, direct_declarator, PUNC1, type_qualifier_list, assignment_expression, PUNC2),
    R(direct_declarator, direct_declarator, PUNC1, type_qualifier_list, PUNC2),
    R(direct_declarator, direct_declarator, PUNC1, assignment_expression, PUNC2),
    R(direct_declarator, direct_declarator, PUNC1, PUNC2),
    R(direct_declarator, direct_declarator, PUNC1, K_STATIC, assignment_expression, PUNC2),
    R(direct_declarator, direct_declarator, PUNC1, K_STATIC, type_qualifier_list, assignment_expression, PUNC2),
    R(direct_declarator, direct_declarator, PUNC1, type_qualifier_list, K_STATIC, assignment_expression, PUNC2),
    R(direct_declarator, direct_declarator, PUNC1, type_qualifier_list, PUNC12, PUNC2),
    R(direct_declarator, direct_declarator, PUNC1, PUNC12, PUNC2),
    R(direct_declarator, direct_declarator, PUNC3, parameter_type_list, PUNC4),
    R(direct_declarator, direct_declarator, PUNC3, identifier_list, PUNC4),
    R(direct_declarator, direct_declarator, PUNC3, PUNC4),
    R(pointer, PUNC12),
    R(pointer, PUNC12, type_qualifier_list),
    R(pointer, PUNC12, pointer),
    R(pointer, PUNC12, type_qualifier_list, pointer),
    R(type_qualifier_list, type_qualifier),
    R(type_qualifier_list, type_qualifier_list, type_qualifier),
    R(parameter_type_list, parameter_list),
    R(parameter_type_list, parameter_list, PUNC46, PUNC34),
    R(parameter_list, parameter_declaration),
    R(parameter_list, parameter_list, PUNC46, parameter_declaration),
    R(parameter_declaration, declaration_specifiers, declarator),
    R(parameter_declaration, declaration_specifiers, abstract_declarator),
    R(parameter_declaration, declaration_specifiers),
    R(identifier_list, identifier_list, PUNC46, IDENT),
    R(identifier_list, IDENT),
    R(type_name, specifier_qualifier_list, abstract_declarator),
    R(type_name, specifier_qualifier_list),
    R(abstract_declarator, pointer),
    R(abstract_declarator, pointer, direct_abstract_declarator),
    R(abstract_declarator, direct_abstract_declarator),
    R(direct_abstract_declarator, PUNC3, abstract_declarator, PUNC4),
    R(direct_abstract_declarator, direct_abstract_declarator_final),
    R(direct_abstract_declarator, direct_abstract_declarator, direct_abstract_declarator_final),
    R(direct_abstract_declarator_final, PUNC1, type_qualifier_list, assignment_expression, PUNC2),
    R(direct_abstract_declarator_final, PUNC1, type_qualifier_list, PUNC2),
    R(direct_abstract_declarator_final, PUNC1, assignment_expression, PUNC2),
    R(direct_abstract_declarator_final, PUNC1, PUNC2),
    R(direct_abstract_declarator_final, PUNC1, K_STATIC, assignment_expression, PUNC2),
    R(direct_abstract_declarator_final, PUNC1, K_STATIC, type_qualifier_list, assignment_expression, PUNC2),
    R(direct_abstract_declarator_final, PUNC1, type_qualifier_list, K_STATIC, assignment_expression, PUNC2),
    R(direct_abstract_declarator_final, PUNC1, PUNC12, PUNC2),
    R(direct_abstract_declarator_final, PUNC3, parameter_type_list, PUNC4),
    R(direct_abstract_declarator_final, PUNC3, PUNC4),
    R(typedef_name, IDENT),
    R(initializer, assignment_expression),
    R(initializer, PUNC5, initializer_list, PUNC6),
    R(initializer, PUNC5, initializer_list, PUNC46, PUNC6),
    R(initializer_list, designation, initializer),
    R(initializer_list, initializer_list, PUNC46, designation, initializer),
    R(initializer_list, initializer),
    R(initializer_list, initializer_list, PUNC46, initializer),
    R(designation, designator_list, PUNC35),
    R(designator_list, designator),
    R(designator_list, designator_list, designator),
    R(designator, PUNC1, constant_expression, PUNC2),
    R(designator, PUNC7, IDENT),
    R(statement, labeled_statement),
    R(statement, compound_statement),
    R(statement, expression_statement),
    R(statement, selection_statement),
    R(statement, iteration_statement),
    R(statement, jump_statement),
    R(labeled_statement, IDENT, PUNC32, statement),
    R(labeled_statement, K_CASE, constant_expression, PUNC32, statement),
    R(labeled_statement, K_DEFAULT, PUNC32, statement),
    R(compound_statement, PUNC5, PUNC6),
    R(compound_statement, PUNC5, block_item_list, PUNC6),
    R(block_item_list, block_item),
    R(block_item_list, block_item_list, block_item),
    R(block_item, declaration),
    R(block_item, statement),
    R(expression_statement, PUNC33),
    R(expression_statement, expression, PUNC33),
    R(selection_statement, K_IF, PUNC3, expression, PUNC4, statement),
    R(selection_statement, K_IF, PUNC3, expression, PUNC4, statement, K_ELSE, statement),
    R(selection_statement, K_SWITCH, PUNC3, expression, PUNC4, statement),
    R(iteration_statement, K_WHILE, PUNC3, expression, PUNC4, statement),
    R(iteration_statement, K_DO, statement, K_WHILE, PUNC3, expression, PUNC4, PUNC33),
    R(iteration_statement, K_FOR, PUNC3, expression, PUNC33, expression, PUNC33, expression, PUNC4, statement),
    R(iteration_statement, K_FOR, PUNC3, PUNC33, expression, PUNC33, expression, PUNC4, statement),
    R(iteration_statement, K_FOR, PUNC3, expression, PUNC33, PUNC33, expression, PUNC4, statement),
    R(iteration_statement, K_FOR, PUNC3, expression, PUNC33, expression, PUNC33, PUNC4, statement),
    R(iteration_statement, K_FOR, PUNC3, PUNC33, PUNC33, expression, PUNC4, statement),
    R(iteration_statement, K_FOR, PUNC3, PUNC33, expression, PUNC33, PUNC4, statement),
    R(iteration_statement, K_FOR, PUNC3, expression, PUNC33, PUNC33, PUNC4, statement),
    R(iteration_statement, K_FOR, PUNC3, PUNC33, PUNC33, PUNC4, statement),
    R(iteration_statement, K_FOR, PUNC3, declaration, expression, PUNC33, expression, PUNC4, statement),
    R(iteration_statement, K_FOR, PUNC3, declaration, PUNC33, expression, PUNC4, statement),
    R(iteration_statement, K_FOR, PUNC3, declaration, expression, PUNC33, PUNC4, statement),
    R(iteration_statement, K_FOR, PUNC3, declaration, PUNC33, PUNC4, statement),
    R(jump_statement, K_GOTO, IDENT, PUNC33),
    R(jump_statement, K_CONTINUE, PUNC33),
    R(jump_statement, K_BREAK, PUNC33),
    R(jump_statement, K_RETURN, expression, PUNC33),
    R(jump_statement, K_RETURN, PUNC33),
    R(translation_unit, external_declaration),
    R(translation_unit, translation_unit, external_declaration),
    R(external_declaration, function_definition),
    R(external_declaration, declaration),
    R(function_definition, declaration_specifiers, declarator, declaration_list, compound_statement),
    R(function_definition, declaration_specifiers, declarator, compound_statement),
    R(declaration_list, declaration),
    R(declaration_list, declaration_list, declaration),
};
