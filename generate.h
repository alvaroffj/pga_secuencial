#ifndef _GENERATE_H_
#define _GENERATE_H_

void  initpop(int tipo_problema, int corrida);
void  mutation(struct individual *critter);
int   iBuscaPieza( unsigned short uiPieza, unsigned short *puiPiezas );
void  IntercambiaPiezas( int i, int j, unsigned short *puiPiezas );
int   crossover (unsigned *parent1,unsigned *parent2,unsigned *child1,unsigned *child2,
                 unsigned *mparent1,unsigned *mparent2,unsigned *mchild1,unsigned *mchild2,
                 unsigned short *lparent1,unsigned short *lparent2,unsigned short *lchild1,unsigned short *lchild2);
void  xcrossover(unsigned *parent1,unsigned *parent2,unsigned *child1,unsigned *child2,
					  unsigned *mparent1,unsigned *mparent2,unsigned *mchild1,unsigned *mchild2);
void  preselect(void);
int   xselect(void);
int   tournament(void);
void  generation(int tipo_problema, int corrida);
void  statistics(struct individual *pop, int corrida);

#endif
