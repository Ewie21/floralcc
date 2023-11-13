#include"cgen.h"

// #define add_comma(ret) ret = strcat(ret, ", ")
// // Inits new "ret"
#define TOK_IS_OP(t) t == TOK_B_AND || t == TOK_B_OR || t == TOK_B_XOR || t == TOK_ADD || t == TOK_SUB || t == TOK_DIV || t == TOK_MUL

// D: never used this D:
#define push(reg, size) \
    sprintf(ret, "%s\nstr %s, [sp, #-%s]", ret, reg, size);
#define pop(reg, size) \
    sprintf(ret, "%s\nldr %s, [sp], #%s", ret, reg, size);

// Time to iterate over the tree depth first
char* code_gen(TokenNode* parent) {
    char* ret;
    RegisterTracker* tracker = new_reg_tracker();
    if (parent->token->type == TOK_PROGRAM) {
        tracker->sp = parent->children->len; // find how to calculate this number in advance, fornow I will use the number of children of the parent, althoug this will break down once loops and conditionals are implemented here
        TokenNode* node = (TokenNode*)get_vec(parent->children, 0);
        if (node->token->type == TOK_DECLARE) { // This is un-dynamic code that needs to be refactored
            Vec* declare_result = declare_code_gen(node, tracker);
        } else if (1) { // placeholder so the linter shuts up
            // Other potential things like statements
        }
    }
    return ret;
}

// Returns the code and the memory address of the register
Vec* declare_code_gen(TokenNode* parent, RegisterTracker* tracker) { // remember that this will be called many times
    if (((TokenNode*)get_vec(parent->children, 0))->token->type != TOK_ASSIGN) return NULL;
    else parent = get_vec(parent->children, 0); // moves past the assignment token
    char* ret;
    Vec* res = expr_code_gen(get_vec(parent->children, 0), tracker);
    int* reg = (int*)get_vec(res, 1);
    char* code = get_vec(res, 0);
    int adr = tracker->sp -= 4; // comes from decrementing the size of the stack, which needs to be decided on before hand.
    sprintf(ret, "%s\n%s", ret, code);
    sprintf(ret, "%s\nstr x%d, [sp, %d]", ret, *reg, adr);
    return res;
}

// Right now we check the parent
// Evaluates an expression into the lowest avaliable register: reg_A.
// Also needs to store the register the end value gets stored in
Vec* expr_code_gen(TokenNode* parent, RegisterTracker* tracker) {
    // An operator represents the begining of an expression
    char* ret = "";
    Vec* ret_vec = new_vec(2);
    push_vec(ret_vec, ret);
    if (TOK_IS_OP(parent->token->type)) {
        Vec* children = parent->children;

        TokenNode* left = get_vec(children, 0);
        char* left_res = (char*)get_vec(expr_code_gen(left, tracker), 0);
        sprintf(ret, "%s\n%s", ret, left_res);

        TokenNode* right = NULL;
        if (children->len == 2) {
            right = get_vec(children, 1);
            char* right_res = (char*)get_vec(expr_code_gen(right, tracker), 0);
            sprintf(ret, "%s\n%s", ret, right_res);
        }
        // Then perform operation and mov into new register that we keep track of
        // might refactor to deal with parent's roots, and not recurse parent into value
        int reg_A = *assign_register(tracker, REG_GEN_X); // assign register needs a refactor for this to work
        int reg_B = *assign_register(tracker, REG_GEN_X);
        switch (parent->token->type) {
            case TOK_B_AND:
                sprintf(ret, "%s\nand x%d, x%d, x%d", ret, reg_A, reg_B, reg_A); // hopefully this doesn't doesn't modify the pointer
            case TOK_B_OR:
                sprintf(ret, "%s\norr x%d, x%d, x%d", ret, reg_A, reg_B, reg_A);
            case TOK_B_XOR:
                sprintf(ret, "%s\eor x%d, x%d, x%d", ret, reg_A, reg_B, reg_A);
            case TOK_ADD:
                sprintf(ret, "%s\nadd x%d, x%d, x%d", ret, reg_A, reg_B, reg_A);
            case TOK_SUB:
                sprintf(ret, "%s\nsub x%d, x%d, x%d", ret, reg_A, reg_B, reg_A);
            case TOK_MUL:
                sprintf(ret, "%s\nmul x%d, x%d, x%d", ret, reg_A, reg_B, reg_A);
            case TOK_DIV:
                sprintf(ret, "%s\nsdiv x%d, x%d, x%d", ret, reg_A, reg_B, reg_A);
        }
        free_reg(tracker, REG_GEN_X, reg_B);
        push_vec(ret_vec, &reg_A); // Leaks memory
    } else {
        char* reg_buff;
        itoa(*assign_register(tracker, REG_GEN_X), reg_buff, 10);
        if (parent->token->type == TOK_NUM) {
            sprintf(ret, "mov x%s, %s", reg_buff, (char*)parent->token->value);
        } else if (parent->token->type == TOK_VAR) {
            // TODO: figure out how to record the addresses of variables, put them on the stack, figure out how to load that back properly
        }
    }
    return ret_vec;
}

char* itoa(int num, char* buffer, int base) {
    int current = 0;
    if (num == 0) { 
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;  
    }
    int num_digits = 0;  
    if (num < 0) {  
        if (base == 10) {  
            num_digits++;
            buffer[0] = '-';
            current++; 
            num *= -1;
        } else return NULL;
    }  
    num_digits += (int)floor(log(num) / log(base)) + 1;  
    while (current < num_digits) {  
        int base_val = (int) pow(base, num_digits-1-current);  
        int num_val = num / base_val;  
        char value = num_val + '0';  
        buffer[current] = value;  
        current++;  
        num -= base_val * num_val;  
    }  
    buffer[current] = '\0';  
    return buffer;  
}