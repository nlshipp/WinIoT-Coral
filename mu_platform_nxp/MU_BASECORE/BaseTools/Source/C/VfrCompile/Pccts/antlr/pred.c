/*
 * pred.c -- source for predicate detection, manipulation
 *
 * SOFTWARE RIGHTS
 *
 * We reserve no LEGAL rights to the Purdue Compiler Construction Tool
 * Set (PCCTS) -- PCCTS is in the public domain.  An individual or
 * company may do whatever they wish with source code distributed with
 * PCCTS or the code generated by PCCTS, including the incorporation of
 * PCCTS, or its output, into commerical software.
 *
 * We encourage users to develop software with PCCTS.  However, we do ask
 * that credit is given to us for developing PCCTS.  By "credit",
 * we mean that if you incorporate our source code into one of your
 * programs (commercial product, research project, or otherwise) that you
 * acknowledge this fact somewhere in the documentation, research report,
 * etc...  If you like PCCTS and have developed a nice tool with the
 * output, please mention that you developed it using PCCTS.  In
 * addition, we ask that this header remain intact in our source code.
 * As long as these guidelines are kept, we expect to continue enhancing
 * this system and expect to make other tools available as they are
 * completed.
 *
 * ANTLR 1.33
 * Terence Parr
 * Parr Research Corporation
 * with Purdue University and AHPCRC, University of Minnesota
 * 1989-2001
 */

#include <stdio.h>
#include "pcctscfg.h"
#include "set.h"
#include "syn.h"
#include "hash.h"
#include "generic.h"
#include "dlgdef.h"
#include <ctype.h>

#ifdef __USE_PROTOS
static void complete_context_sets(RuleRefNode *, Predicate *);
static void complete_context_trees(RuleRefNode *, Predicate *);
#else
static void complete_context_sets();
static void complete_context_trees();
#endif

char *PRED_AND_LIST = "AND";
char *PRED_OR_LIST = "OR";

/*
 * In C mode, return the largest constant integer found as the
 * sole argument to LATEXT(i).
 *
 * In C++ mode, return the largest constant integer found as the
 * sole argument to LT(i) given that the char before is nonalpha.
 */

int
#ifdef __USE_PROTOS
predicateLookaheadDepth(ActionNode *a)
#else
predicateLookaheadDepth(a)
ActionNode *a;
#endif
{
	int     max_k=0;

    if (a->predEntry != NULL) {
       MR_pred_depth(a->predEntry->pred,&max_k);
       goto PREDENTRY_EXIT;
    }

	if ( GenCC )
	{
		/* scan for LT(i) */
		int k = 0;
		char *p = a->action;
		while ( p!=NULL )
		{
			p = strstr(p, "LT(");
			if ( p!=NULL )
			{
				if ( p>=a->action && !isalpha(*(p-1)) )
				{
					k = atoi(p+strlen("LT("));
					if ( k>max_k ) max_k=k;
				}
				p += strlen("LT(");
			}
		}
	}
	else {
		/* scan for LATEXT(i) */
		int k = 0;
		char *p = a->action;
		while ( p!=NULL )
		{
			p = strstr(p, "LATEXT(");
			if ( p!=NULL )
			{
				p += strlen("LATEXT(");
				k = atoi(p);
				if ( k>max_k ) max_k=k;
			}
		}
	}

	if (max_k==0) {
		max_k = 1;	   /* MR33 Badly designed if didn't set max_k when CLL_k = 1 */
		if (CLL_k > 1) /* MR27 Don't warn if max(k,ck) == 1 */
		{
			if ( !a->frmwarned )
			{
				a->frmwarned = 1;
				warnFL(eMsg1("predicate: %s missing, bad, or with i=0; assuming i=1",
							 GenCC?"LT(i)":"LATEXT(i)"),
					   FileStr[a->file], a->line);
			}
		}
	}

/* MR10 */    if ( max_k > CLL_k) {
/* MR10 */		if ( !a->frmwarned )
/* MR10 */        {
/* MR10 */			a->frmwarned = 1;
/* MR11 */ errFL(eMsgd2("predicate refers to lookahead token %d. Semantic lookahead is limited to max(k,ck)==%d",
/* MR10 */                        max_k,CLL_k),
/* MR10 */                        FileStr[a->file],a->line);
/* MR10 */          if (max_k >= OutputLL_k) {
/* MR10 */            if (!GenCC) {
/* MR10 */              errFL(eMsgd("    the lookahead buffer size in C mode is %d token(s) (including the one just recognized)",
/* MR10 */                        OutputLL_k),
/* MR10 */                        FileStr[a->file],a->line);
/* MR10 */            };
/* MR10 */          };
/* MR10 */        };
/* MR10 */        max_k= CLL_k;
/* MR10 */    };

PREDENTRY_EXIT:
	return max_k;
}

/* Find all predicates in a block of alternatives.  DO NOT find predicates
 * behind the block because that predicate could depend on things set in
 * one of the nonoptional blocks
 */

Predicate *
#ifdef __USE_PROTOS
find_in_aSubBlk( Junction *alt )
#else
find_in_aSubBlk( alt )
Junction *alt;
#endif
{
	Predicate *a, *head=NULL, *tail=NULL, *root=NULL;
	Junction *p = alt;

    if (MRhoisting) {
      return MR_find_in_aSubBlk(alt);
    };
	for (; p!=NULL; p=(Junction *)p->p2)
	{
		/* ignore empty alts */
		if ( p->p1->ntype != nJunction ||
			 ((Junction *)p->p1)->jtype != EndBlk )
		{
			a = find_predicates(p->p1);	/* get preds for this alt */
			if ( a==NULL ) continue;

			/* make an OR list of predicates */
			if ( head==NULL )
			{
				root = new_pred();
				root->expr = PRED_OR_LIST;
				head = tail = a;
				root->down = head;
			}
			else {
				tail->right = a;
				a->left = tail;
				a->up = tail->up;
				tail = a;
			}
		}
	}

	/* if just one pred, remove OR root */
	if ( root!=NULL && root->down->right == NULL )
	{
		Predicate *d = root->down;
		free( (char *) root);
		return d;
	}

	return root;
}

Predicate *
#ifdef __USE_PROTOS
find_in_aOptBlk( Junction *alt )
#else
find_in_aOptBlk( alt )
Junction *alt;
#endif
{
	return find_in_aSubBlk( alt );
}

Predicate *
#ifdef __USE_PROTOS
find_in_aLoopBegin( Junction *alt )
#else
find_in_aLoopBegin( alt )
Junction *alt;
#endif
{
	return find_in_aSubBlk( (Junction *) alt->p1 );	/* get preds in alts */
}

Predicate *
#ifdef __USE_PROTOS
find_in_aPlusBlk( Junction *alt )
#else
find_in_aPlusBlk( alt )
Junction *alt;
#endif
{
	require(alt!=NULL&&alt->p2!=NULL, "invalid aPlusBlk");
	return find_in_aSubBlk( alt );
}

/* Look for a predicate;
 *
 * Do not pass anything but Junction nodes; no Actions, Tokens, RuleRefs.
 * This means that a "hoisting distance" of zero is the only distance
 * allowable.  Init actions are ignored.
 *
 * WARNING:
 *		Assumes no (..)? block after predicate for the moment.
 *		Does not check to see if pred is in production that can generate
 *			a sequence contained in the set of ambiguous tuples.
 *
 * Return the predicate found if any.
 */


Predicate *
#ifdef __USE_PROTOS
find_predicates( Node *alt )
#else
find_predicates( alt )
Node *alt;
#endif
{
#ifdef DBG_PRED
	Junction *j;
	RuleRefNode *r;
	TokNode *t;
#endif
	Predicate *pred;

	if ( alt==NULL ) return NULL;

#ifdef DBG_PRED
	switch ( alt->ntype )
	{
		case nJunction :
			j = (Junction *) alt;
			fprintf(stderr, "Junction(in %s)", j->rname);
			switch ( j->jtype )
			{
				case aSubBlk :
					fprintf(stderr,"aSubBlk\n");
					break;
				case aOptBlk :
					fprintf(stderr,"aOptBlk\n");
					break;
				case aLoopBegin :
					fprintf(stderr,"aLoopBeginBlk\n");
					break;
				case aLoopBlk :
					fprintf(stderr,"aLoopBlk\n");
					break;
				case aPlusBlk :
					fprintf(stderr,"aPlusBlk\n");
					break;
				case EndBlk :
					fprintf(stderr,"EndBlk\n");
					break;
				case RuleBlk :
					fprintf(stderr,"RuleBlk\n");
					break;
				case Generic :
					fprintf(stderr,"Generic\n");
					break;
				case EndRule :
					fprintf(stderr,"EndRule\n");
					break;
			}
			break;
		case nRuleRef :
			r = (RuleRefNode *) alt;
			fprintf(stderr, "RuleRef(in %s)\n", r->rname);
			break;
		case nToken :
			t = (TokNode *) alt;
			fprintf(stderr, "TokenNode(in %s)%s\n", t->rname, TokenString(t->token));
			break;
		case nAction :
			fprintf(stderr, "Action\n");
			break;
	}
#endif

	switch ( alt->ntype )
	{
		case nJunction :
		{
			Predicate *a, *b;
			Junction *p = (Junction *) alt;

			/* lock nodes */
			if ( p->jtype==aLoopBlk || p->jtype==RuleBlk ||
				 p->jtype==aPlusBlk || p->jtype==EndRule )
			{
				require(p->pred_lock!=NULL, "rJunc: lock array is NULL");
				if ( p->pred_lock[1] )
				{
					return NULL;
				}
				p->pred_lock[1] = TRUE;
			}

			switch ( p->jtype )
			{
				case aSubBlk :
					a = find_in_aSubBlk(p);
					return a;	/* nothing is visible past this guy */
				case aOptBlk :
					a = find_in_aOptBlk(p);
					return a;
				case aLoopBegin :
					a = find_in_aLoopBegin(p);
					return a;
				case aLoopBlk :
					a = find_in_aSubBlk(p);
					p->pred_lock[1] = FALSE;
					return a;
				case aPlusBlk :
					a = find_in_aPlusBlk(p);
					p->pred_lock[1] = FALSE;
					return a;	/* nothing is visible past this guy */
				case RuleBlk :
					a = find_predicates(p->p1);
					p->pred_lock[1] = FALSE;
					return a;
				case Generic :
					a = find_predicates(p->p1);
					b = find_predicates(p->p2);
					if ( p->pred_lock!=NULL ) p->pred_lock[1] = FALSE;
					if ( a==NULL ) return b;
					if ( b==NULL ) return a;
					/* otherwise OR the two preds together */
					{
					fatal_internal("hit unknown situation during predicate hoisting");
					}
				case EndBlk :
				case EndRule :	/* Find no predicates after a rule ref */
					return NULL;
				default:
					fatal_internal("this cannot be printed\n");
					break;
			}
		}
		case nAction :
		{
			ActionNode *p = (ActionNode *) alt;
            if ( p->noHoist) return NULL;                           /* MR12c */
			if ( p->init_action ) return find_predicates(p->next);
			if ( p->is_predicate )
			{
				Tree *t=NULL;
#ifdef DBG_PRED
				fprintf(stderr, "predicate: <<%s>>?\n", p->action);
#endif
				if ( p->guardpred!=NULL )
				{
					pred = predicate_dup(p->guardpred);
                    MR_guardPred_plainSet(p,pred);                  /* MR12c */
				}
				else
				{
					pred = new_pred();
					pred->k = predicateLookaheadDepth(p);
					pred->source = p;
					pred->expr = p->action;
					if ( HoistPredicateContext && pred->k > 1 )
					{
						/* MR30 No need to use first_item_is_guess_block_extra
						   since we know this is an action, not a (...)* or
						   (...)+ block.
						*/

						if ( first_item_is_guess_block((Junction *)p->next) )
						{
                            warnFL("cannot compute context of predicate in front of (..)? block",
                            FileStr[p->file], p->line);
						}
						else
						{
							ConstrainSearch = 0;
/* MR11 */                  if (p->ampersandPred != NULL) {
/* MR11 */  					TRAV(p,
/* MR11 */							 pred->k,
/* MR11 */  						 &(pred->completionTree), t);
/* MR11 */                  } else {
							TRAV(p->next,
								 pred->k,
								 &(pred->completionTree), t);
                            };
							pred->tcontext = t;
                            MR_check_pred_too_long(pred,pred->completionTree);
#ifdef DBG_PRED
							fprintf(stderr, "LL(%d) context:", pred->k);
							preorder(t);
							fprintf(stderr, "\n");
#endif
						}
					}
					else if ( HoistPredicateContext && pred->k == 1 )
					{
						pred->scontext[1] = empty;
						/* MR30 No need to use first_item_is_guess_block_extra
						   since we know this is an action.
						*/
						if ( first_item_is_guess_block((Junction *)p->next) )
						{
                        warnFL("cannot compute context of predicate in front of (..)? block",
                                             FileStr[p->file], p->line);
						}
						else
						{
							REACH((Junction *)p->next,
								  1,
								  &(pred->completionSet),
								  pred->scontext[1]);
                            MR_check_pred_too_long(pred,pred->completionSet);
#ifdef DBG_PRED
							fprintf(stderr, "LL(1) context:");
							s_fprT(stderr, pred->scontext[1]);
							fprintf(stderr, "\n");
#endif
						}
					}
				}
				{
					Predicate  *d = find_predicates(p->next);
                    Predicate  *root;

/* Warning: Doesn't seem like the up pointers will all be set correctly;
 * TJP: that's ok, we're not using them now.
 */
					if ( d!=NULL )
					{
						root = new_pred();
						root->expr = PRED_AND_LIST;
						root->down = pred;
						pred->right = d;
						pred->up = root;
						d->left = pred;
						d->up = pred->up;
						return root;
					}
				}
				return pred;
			}
			return NULL;
		}
		case nRuleRef :
		{
			Predicate   *a;
			RuleRefNode *p = (RuleRefNode *) alt;
			Junction    *r;
            Junction    *save_MR_RuleBlkWithHalt;

			RuleEntry *q = (RuleEntry *) hash_get(Rname, p->text);
			if ( q == NULL )
			{
				warnFL( eMsg1("rule %s not defined",p->text), FileStr[p->file], p->line );
				return NULL;
			}
			r = RulePtr[q->rulenum];
			if ( r->pred_lock[1] )
			{
				/* infinite left-recursion; ignore 'cause LL sup 1 (k) analysis
				 * must have seen it earlier.
				 */
				return NULL;
			}

            /* MR10 There should only be one halt set at a time.        */
            /* MR10 Life would have been easier with a global variable  */
            /* MR10    (at least for this particular need)              */
            /* MR10 Unset the old one and set the new one, later undo.  */

            require(r->end->halt == FALSE,"should only have one halt at a time");

/* MR10 */  require(MR_RuleBlkWithHalt == NULL ||
/* MR10 */          (MR_RuleBlkWithHalt->jtype == RuleBlk && MR_RuleBlkWithHalt->end->halt == TRUE),
/* MR10 */             "RuleBlkWithHalt->end not RuleBlk or does not have halt set");
/* MR10 */  if (MR_RuleBlkWithHalt != NULL) {
/* MR10 */    MR_RuleBlkWithHalt->end->halt=FALSE;
/* MR10 */  };

/***        fprintf(stderr,"\nSetting halt on junction #%d\n",r->end->seq);     ***/

            require(r->end->halt == FALSE,"rule->end->halt already set");

            save_MR_RuleBlkWithHalt=MR_RuleBlkWithHalt;

/* MR10 */  MR_pointerStackPush(&MR_RuleBlkWithHaltStack,MR_RuleBlkWithHalt);
/* MR10 */  MR_pointerStackPush(&MR_PredRuleRefStack,p);

			r->end->halt = TRUE;
/* MR10 */  MR_RuleBlkWithHalt=r;

			a = find_predicates((Node *)r);

            require(r->end->halt == TRUE,"rule->end->halt not set");
            r->end->halt = FALSE;

/* MR10 */  MR_pointerStackPop(&MR_PredRuleRefStack);
/* MR10 */  MR_RuleBlkWithHalt=(Junction *) MR_pointerStackPop(&MR_RuleBlkWithHaltStack);

            require (MR_RuleBlkWithHalt==save_MR_RuleBlkWithHalt,
                    "RuleBlkWithHaltStack not consistent");

/* MR10 */  require(MR_RuleBlkWithHalt == NULL ||
/* MR10 */          (MR_RuleBlkWithHalt->jtype == RuleBlk && MR_RuleBlkWithHalt->end->halt == FALSE),
/* MR10 */             "RuleBlkWithHalt->end not RuleBlk or has no halt set");
/* MR10 */  if (MR_RuleBlkWithHalt != NULL) {
/* MR10 */    MR_RuleBlkWithHalt->end->halt=TRUE;
/* MR10 */  };

/***        fprintf(stderr,"\nRestoring halt on junction #%d\n",r->end->seq);   ***/

			if ( a==NULL ) return NULL;

			/* attempt to compute the "local" FOLLOW just like in normal lookahead
			 * computation if needed
			 */

			complete_context_sets(p,a);
			complete_context_trees(p,a);

/* MR10 */  MR_cleanup_pred_trees(a);

			return a;
		}
		case nToken :
			break;
	}

	return NULL;
}

#ifdef __USE_PROTOS
Predicate *MR_find_predicates_and_supp(Node *alt)
#else
Predicate *MR_find_predicates_and_supp(alt)
  Node      *alt;
#endif
{
    Predicate   *p;

    p=find_predicates(alt);
    p=MR_suppressK(alt,p);
    return p;
}

Predicate *
#ifdef __USE_PROTOS
new_pred( void )
#else
new_pred( )
#endif
{
	Predicate *p = (Predicate *) calloc(1,sizeof(Predicate)); /* MR10 */
	require(p!=NULL, "new_pred: cannot alloc predicate");
    p->scontext[0]=empty;
    p->scontext[1]=empty;
    p->completionTree=empty;
    p->completionSet=empty;
    p->plainSet=empty;
	return p;
}

static void
#ifdef __USE_PROTOS
complete_context_sets( RuleRefNode *p, Predicate *a )
#else
complete_context_sets( p, a )
RuleRefNode *p;
Predicate *a;
#endif
{
	set rk2, b;
	int k2;

#ifdef DBG_PRED
	fprintf(stderr, "enter complete_context_sets\n");
#endif
	for (; a!=NULL; a=a->right)
	{
		if ( a->expr == PRED_AND_LIST || a->expr == PRED_OR_LIST )
		{
			complete_context_sets(p,a->down);
			continue;
		}
		rk2 = b = empty;
		while ( !set_nil(a->completionSet) )
		{
			k2 = set_int(a->completionSet);
			set_rm(k2, a->completionSet);

            REACH(p->next, k2, &rk2, b);
			set_orin(&(a->scontext[1]), b);
			set_free(b);
		}

		set_orin(&(a->completionSet), rk2);/* remember what we couldn't do */
		set_free(rk2);
#ifdef DBG_PRED
		fprintf(stderr, "LL(1) context for %s(addr 0x%x) after ruleref:", a->expr, a);
		s_fprT(stderr, a->scontext[1]);
		fprintf(stderr, "\n");
#endif
/*		complete_context_sets(p, a->down);*/
	}
#ifdef DBG_PRED
	fprintf(stderr, "exit complete_context_sets\n");
#endif
}

static void
#ifdef __USE_PROTOS
complete_context_trees( RuleRefNode *p, Predicate *a )
#else
complete_context_trees( p, a )
RuleRefNode *p;
Predicate *a;
#endif
{
	set rk2;
	int k2;
	Tree *u;

#ifdef DBG_PRED
	fprintf(stderr, "enter complete_context_trees\n");
#endif
	for (; a!=NULL; a=a->right)
	{
		if ( a->expr == PRED_AND_LIST || a->expr == PRED_OR_LIST )
		{
			complete_context_trees(p, a->down);
			continue;
		}
		rk2 = empty;

		/* any k left to do? if so, link onto tree */
		while ( !set_nil(a->completionTree) )
		{
			k2 = set_int(a->completionTree);
			set_rm(k2, a->completionTree);
			u = NULL;

            TRAV(p->next, k2, &rk2, u);

			/* any subtrees missing k2 tokens, add u onto end */
			a->tcontext = tlink(a->tcontext, u, k2);
            Tfree(u);   /* MR10 */
		}
		set_orin(&(a->completionTree), rk2);/* remember what we couldn't do */
		set_free(rk2);
#ifdef DBG_PRED
		fprintf(stderr, "LL(i<%d) context after ruleref:", LL_k);
		preorder(a->tcontext);
		fprintf(stderr, "\n");
#endif
/*		complete_context_trees(p, a->down);*/
	}
#ifdef DBG_PRED
	fprintf(stderr, "exit complete_context_trees\n");
#endif
}

/* Walk a list of predicates and return the set of all tokens in scontext[1]'s */
set
#ifdef __USE_PROTOS
covered_set( Predicate *p )
#else
covered_set( p )
Predicate *p;
#endif
{
	set a;

	a = empty;
	for (; p!=NULL; p=p->right)
	{
		if ( p->expr == PRED_AND_LIST || p->expr == PRED_OR_LIST )
		{
			set_orin(&a, covered_set(p->down));
			continue;
		}
		set_orin(&a, p->scontext[1]);
		set_orin(&a, covered_set(p->down));
	}
	return a;
}

/* MR10 predicate_free()
   MR10 Don't free the leaf nodes since they are part of the action node
*/

#ifdef __USE_PROTOS
void predicate_free(Predicate *p)
#else
void predicate_free(p)
  Predicate     *p;
#endif
{
  if (p == NULL) return;
  predicate_free(p->right);
  predicate_free(p->down);
  if (p->cloned ||
      p->source == NULL ||
      p->source->guardpred == NULL ||
      p->expr == PRED_AND_LIST ||
      p->expr == PRED_OR_LIST) {
    set_free(p->scontext[1]);
    set_free(p->completionSet);
    set_free(p->completionTree);
    set_free(p->plainSet);
    Tfree(p->tcontext);
    free( (char *) p);
  } else {
    p->right=NULL;
    p->down=NULL;  /* MR13 *** debug */
  };
}

/* MR10 predicate_dup() */

#ifdef __USE_PROTOS
Predicate * predicate_dup_xxx(Predicate *p,int contextToo)
#else
Predicate * predicate_dup_xxx(p,contextToo)
  Predicate     *p;
  int           contextToo;
#endif
{
  Predicate     *q;

  if (p == NULL) return NULL;
  q=new_pred();
  q->down=predicate_dup(p->down);
  q->right=predicate_dup(p->right);

  /*
     don't replicate expr - it is read-only
     and address comparison is used to look
     for identical predicates.
  */

  q->expr=p->expr;
  q->k=p->k;
  q->source=p->source;
  q->cloned=1;
  q->ampersandStyle=p->ampersandStyle;
  q->inverted=p->inverted;
  q->predEntry=p->predEntry;
  q->plainSet=set_dup(p->plainSet);

  if (contextToo) {
    q->tcontext=tdup(p->tcontext);
    q->scontext[0]=set_dup(p->scontext[0]);
    q->scontext[1]=set_dup(p->scontext[1]);
    q->completionTree=set_dup(p->completionTree);
    q->completionSet=set_dup(p->completionSet);
  };

  /* don't need to dup "redundant" */

  return q;

}

#ifdef __USE_PROTOS
Predicate * predicate_dup_without_context(Predicate *p)
#else
Predicate * predicate_dup_without_context(p)
  Predicate     *p;
#endif
{
  return predicate_dup_xxx(p,0);
}

#ifdef __USE_PROTOS
Predicate * predicate_dup(Predicate *p)
#else
Predicate * predicate_dup(p)
  Predicate     *p;
#endif
{
  return predicate_dup_xxx(p,1);
}
