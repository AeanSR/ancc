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

