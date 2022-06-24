/*--------------------------------------------------------------------*/
/* decomment.c                                                        */
/* Author: Catherine Yu                                               */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

/* Declare enumeration constants for DFA states. */
enum Statetype {NORMAL, MAYBE_COMMENT, IN_COMMENT, MAYBE_NORMAL,
                IN_CHAR, CHAR_ESCAPE, IN_STRING, STRING_ESCAPE};

/*--------------------------------------------------------------------*/

/* Implement the NORMAL state of the DFA. c is the current DFA charact-
   er. Write c, as specified by the DFA. Return the next state. */

enum Statetype handleNormalState(int c)
{
   enum Statetype state;
   switch (c)
   {
      case '/': 
         state = MAYBE_COMMENT;
         break;
      case '\'': 
         putchar(c);
         state = IN_CHAR;
         break;
      case '\"':
         putchar(c);
         state = IN_STRING;
         break;
      default:
         putchar(c);
         state = NORMAL;
         break; 
   }
   return state;
}

/*--------------------------------------------------------------------*/

/* Implement the MAYBE_COMMENT state of the DFA. c is the current DFA
   character. Write c to stdout, as specified by the DFA. Return the 
   next state. */

enum Statetype handleMaybeCommentState(int c)
{
   enum Statetype state;
   switch (c)
   {
      case '*':
         putchar(' ');
         state = IN_COMMENT;
         break;
      case '/':
         putchar(c);
         state = MAYBE_COMMENT;
         break;
      case '\'':
         putchar('/');
         putchar(c);
         state = IN_CHAR;
         break;
      case '\"':
         putchar('/');
         putchar(c);
         state = IN_STRING;
         break;
      default:
         putchar('/');
         putchar(c);
         state = NORMAL;
         break;
   }
   return state;
}

/*--------------------------------------------------------------------*/

/* Implement the IN_COMMENT state of the DFA. c is the current DFA 
   character. Write c to stdout, as specified by the DFA. Return the 
   next state. */

enum Statetype handleInCommentState(int c)
{
   enum Statetype state;
   switch(c)
   {
      case '*':
         state = MAYBE_NORMAL;
         break;
      case '\n':
         putchar(c);
         state = IN_COMMENT;
         break;
      default:
         state = IN_COMMENT;
         break;
   }
   return state;
}

/*--------------------------------------------------------------------*/

/* Implement the MAYBE_NORMAL state of the DFA. c is the current DFA
   character. Write c to stdout, as specified by the DFA. Return the
   next state. */

enum Statetype handleMaybeNormalState(int c)
{
   enum Statetype state;
   switch (c)
   {
      case '/':
         state = NORMAL;
         break;
      case '*':
         state = MAYBE_NORMAL;
         break;
      case '\n':
         putchar(c);
         state = IN_COMMENT;
         break;
      default:
         state = IN_COMMENT;
         break;
   }
   return state;
}

/*--------------------------------------------------------------------*/

/* Implement the IN_CHAR state of the DFA. c is the current DFA charac-
   ter. Write c to stdout, as specified by the DFA. Return the next 
   state. */

enum Statetype handleInCharState(int c)
{
   enum Statetype state;
   putchar(c);
   switch (c)
   {
      case '\'':
         state = NORMAL;
         break;
      case '\\':
         state = CHAR_ESCAPE;
         break;
      default:
         state = IN_CHAR;
         break;
   }
   return state;
}

/*--------------------------------------------------------------------*/

/* Implement the CHAR_ESCAPE state of the DFA. c is the current DFA
   character. Write c to stdout, as specified by the DFA. Return the 
   next state. */

enum Statetype handleCharEscapeState(int c)
{
   putchar(c);
   return IN_CHAR;
}

/*--------------------------------------------------------------------*/

/* Implement the IN_STRING state of the DFA. c is the current DFA
   character. Write c to stdout, as specified by the DFA. Return the
   next state. */

enum Statetype handleInStringState(int c)
{
   enum Statetype state;
   putchar(c);
   switch (c)
   {
      case '\"':
         state = NORMAL;
         break;
      case '\\':
         state = STRING_ESCAPE;
         break;
      default:
         state = IN_STRING;
         break;
   }
   return state;
}

/*--------------------------------------------------------------------*/

/* Implement the STRING_ESCAPE state of the DFA. c is the current DFA
   character. Write c to stdout, as specified by the DFA. Return the
   next state. */

enum Statetype handleStringEscapeState(int c)
{
   putchar(c);
   return IN_STRING;
}

/*--------------------------------------------------------------------*/

/* Read text from stdin. Remove comments from the text and write the 
   result to stdout. Return EXIT_FAILURE if unterminated comment and 
   print to stderror, otherwise return 0. */

int main(void)
{  int c;
   /* Maintain counters for total lines of text and number of lines for
   an unterminated comment. */
   int total_lines = 1;
   int comment_lines = 0;
   /* Use a DFA approach. state indicates the DFA state. */
   enum Statetype state = NORMAL;
   while ((c = getchar()) != EOF)
   {
      if (c == '\n')
      {
         total_lines++;
      }
      switch (state)
      {  case NORMAL:
            state = handleNormalState(c);
            break;
         case MAYBE_COMMENT:
            state = handleMaybeCommentState(c);
            break;
         case IN_COMMENT:
            if (c == '\n')
            {
               comment_lines++;
            }
            state = handleInCommentState(c);
            break;
         case MAYBE_NORMAL:
            state = handleMaybeNormalState(c);
            if (c == '\n')
            {
               comment_lines++;
            }
            /* Reset count for comment lines if comment terminates. */
            if (state == NORMAL)
            {
               comment_lines = 0;
            }
            break;
         case IN_CHAR:
            state = handleInCharState(c);
            break;
         case CHAR_ESCAPE:
            state = handleCharEscapeState(c);
            break;
         case IN_STRING:
            state = handleInStringState(c);
            break;
         case STRING_ESCAPE:
            state = handleStringEscapeState(c);
            break;
      }  
   }
   if (state == MAYBE_COMMENT)
   {
      putchar('/');
   }
   /* Return EXIT_FAILURE if end in a reject state. Calculate the line 
      number that the unterminated comment starts on and print to 
      stderror. */
   if (state == IN_COMMENT || state == MAYBE_NORMAL)
   {
      int line = total_lines - comment_lines;
      fprintf(stderr, "Error: line %d: unterminated comment\n", line);
      return EXIT_FAILURE;
   }
   return 0;
}
