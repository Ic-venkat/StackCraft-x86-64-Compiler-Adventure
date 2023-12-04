#include <stdio.h>
#include "calc3.h"
#include "y.tab.h"
#include <stdlib.h>

const char *print_format = "%d\n";

static int lbl;
static int break_lbl;
static int continue_lbl;

int ex(nodeType *p) {
    int lbl1, lbl2;

    if (!p) return 0;
    switch(p->type) {
    case typeCon:
        printf("\tpushq	$%d\n", p->con.value);
        break;
    case typeId:
        printf("\tpushq	%c(%%rip)\n", p->id.i + 'a');
        break;
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:
            lbl1 = lbl++;
            lbl2 = lbl++;
            printf("L%03d:\n", lbl1);
            continue_lbl = lbl1;
            break_lbl = lbl2;
            ex(p->opr.op[0]);
            printf("\tpopq\t%%rax\n");
            printf("\ttestq\t%%rax, %%rax\n");
            printf("\tjz\tL%03d\n", lbl2);
            ex(p->opr.op[1]);
            printf("\tjmp\tL%03d\n", lbl1);
            printf("L%03d:\n", lbl2);
            break;
        case IF:
            lbl1 = lbl++;
            lbl2 = lbl++;
            ex(p->opr.op[0]);
            printf("\tpopq\t%%rax\n");
            printf("\ttestq\t%%rax, %%rax\n");
            if (p->opr.nops > 2) {
                printf("\tjz\tL%03d\n", lbl1);
                ex(p->opr.op[1]);
                printf("\tjmp\tL%03d\n", lbl2);
                printf("L%03d:\n", lbl1);
                ex(p->opr.op[2]);
                printf("L%03d:\n", lbl2);
            } else {
                printf("\tjz\tL%03d\n", lbl1);
                ex(p->opr.op[1]);
                printf("L%03d:\n", lbl1);
            }
            break;
        case PRINT:
            ex(p->opr.op[0]);
            printf("\tpopq\t%%rsi\n");  // Pop value to be printed into rsi
            printf("\tsubq\t$8, %%rsp\n");  // Align the stack
            printf("\tleaq\t.LC0(%%rip), %%rdi\n");  // Load format string
            printf("\txorq\t%%rax, %%rax\n");  // Clear rax (varargs count for printf)
            printf("\tcall\tprintf\n");
            printf("\taddq\t$8, %%rsp\n");  // Restore the stack
            break;
        case '=':
            ex(p->opr.op[1]);
            printf("\tpopq	%c(%%rip)\n", p->opr.op[0]->id.i + 'a');
            break;
        case UMINUS:
            ex(p->opr.op[0]);
            printf("\tpopq\t%%rax\n");
            printf("\tneg\t%%rax\n");
            printf("\tpushq\t%%rax\n");
            break;
        // ... (Newly added functions: FACT, LNTWO)
        case GCD:
            printf("\t# Begin gcd function\n");
            printf("\tpopq\t%%rbx\n");  // Load second value into rbx
            printf("\tpopq\t%%rax\n");  // Load first value into rax
            printf("L_gcd_loop%03d:\n", lbl);
            printf("\ttestq\t%%rbx, %%rbx\n");  // Test if rbx is 0
            printf("\tjz\tL_gcd_end%03d\n", lbl);
            printf("\tmovq\t%%rax, %%rcx\n");  // Copy rax to rcx
            printf("\tmovq\t%%rbx, %%rax\n");  // Move rbx to rax
            printf("\tremq\t%%rcx, %%rax\n");  // rax = rax % rcx
            printf("\tmovq\t%%rax, %%rbx\n");  // Move remainder to rbx
            printf("\tjmp\tL_gcd_loop%03d\n", lbl);
            printf("L_gcd_end%03d:\n", lbl);
            printf("\tpushq\t%%rcx\n");  // Push result onto stack
            lbl++;
            printf("\t# End gcd function\n");
            break;
        default:
            ex(p->opr.op[0]);
            ex(p->opr.op[1]);
            switch(p->opr.oper) {
            case '+':
                printf("\tpopq	%%rbx\n");
                printf("\tpopq	%%rax\n");
                printf("\taddq	%%rbx, %%rax\n");
                printf("\tpushq	%%rax\n");
                break;
            case '-':
                printf("\tpopq	%%rbx\n");
                printf("\tpopq	%%rax\n");
                printf("\tsubq	%%rbx, %%rax\n");
                printf("\tpushq	%%rax\n");
                break;
            case '*':
                printf("\tpopq	%%rbx\n");
                printf("\tpopq	%%rax\n");
                printf("\timulq	%%rbx\n");
                printf("\tpushq	%%rax\n");
                break;
            case '/':
                printf("\tpopq	%%rbx\n");
                printf("\tpopq	%%rax\n");
                printf("\tcqo\n");
                printf("\tidivq	%%rbx\n");
                printf("\tpushq	%%rax\n");
                break;
            case '<':
                printf("\tpopq\t%%rbx\n");
                printf("\tpopq\t%%rax\n");
                printf("\tcmpq\t%%rbx, %%rax\n");
                printf("\tsetl\t%%al\n");
                printf("\tmovzbq\t%%al, %%rax\n");
                printf("\tpushq\t%%rax\n");
                break;
            case '>':
                printf("\tpopq\t%%rbx\n");
                printf("\tpopq\t%%rax\n");
                printf("\tcmpq\t%%rbx, %%rax\n");
                printf("\tsetg\t%%al\n");
                printf("\tmovzbq\t%%al, %%rax\n");
                printf("\tpushq\t%%rax\n");
                break;
            case GE:
                printf("\tpopq\t%%rbx\n");
                printf("\tpopq\t%%rax\n");
                printf("\tcmpq\t%%rbx, %%rax\n");
                printf("\tsetge\t%%al\n");
                printf("\tmovzbq\t%%al, %%rax\n");
                printf("\tpushq\t%%rax\n");
                break;
            case LE:
                printf("\tpopq\t%%rbx\n");
                printf("\tpopq\t%%rax\n");
                printf("\tcmpq\t%%rbx, %%rax\n");
                printf("\tsetle\t%%al\n");
                printf("\tmovzbq\t%%al, %%rax\n");
                printf("\tpushq\t%%rax\n");
                break;
            case NE:
                printf("\tpopq\t%%rbx\n");
                printf("\tpopq\t%%rax\n");
                printf("\tcmpq\t%%rbx, %%rax\n");
                printf("\tsetne\t%%al\n");
                printf("\tmovzbq\t%%al, %%rax\n");
                printf("\tpushq\t%%rax\n");
                break;
            case EQ:
                printf("\tpopq\t%%rbx\n");
                printf("\tpopq\t%%rax\n");
                printf("\tcmpq\t%%rbx, %%rax\n");
                printf("\tsete\t%%al\n");
                printf("\tmovzbq\t%%al, %%rax\n");
                printf("\tpushq\t%%rax\n");
                break;
            }
        }
    }
    return 0;
}