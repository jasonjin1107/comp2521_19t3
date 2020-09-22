#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

#include "textbuffer.h"

typedef struct textbuffer {
    struct Node *first;
    struct Node *last;
    int numOfNodes;     
 
} textbuffer;

//typedef struct textbuffer *TB;

typedef struct Node {
    struct Node *next;
    struct Node *prev;
    int lineNum;
    char *string; 
} Node; 

/**
 * Allocate a new textbuffer whose contents is initialised with the text
 * in the given string.
 */

//the function creating an empty text buffer
static TB createTB (void) {
    TB new = malloc(sizeof(textbuffer));
    new->first = new->last = NULL;
    new->numOfNodes = 0;
    return new;
}

//creating a new node by copy the needed text and sort the linked list
static void newNode(char *text, TB buffer){
    buffer->numOfNodes++;
    Node *new = malloc(sizeof(Node));
    new->next = new->prev = NULL;
    new->string = NULL;
    int length = strlen(text);
    if (buffer->first == NULL) {      
        buffer->first = new;
        buffer->last = buffer->first;       
    } else {
        buffer->last->next = new;
        new->prev = buffer->last;
        buffer->last = new;     
    }
    new->string = (length == 0) ? malloc(sizeof(char)) : malloc(sizeof(char) * (length + 1));
    if (length == 0)
        strcpy(new->string, "");
    else
        strcpy(new->string, text);
    return;     
} 

//give each line in the buffer a line index number starting from 1
static void giveLineNum (TB tb) {
    int i = 1;
    struct Node *curr = tb->first;
    while (curr != NULL) {
        curr->lineNum = i;
        curr = curr->next;
        i++;
    }
}

//creating a new textbuffer with all strings added 
TB newTB (char *text) { 
    TB new = createTB();
    int length = strlen(text);
    if (length == 0) {
        return new;
    }
    char array[length + 1];
    strcpy(array, text);
    int i = 0;
    int flag = 0, newFlag = 0;
    char *pToken;
    while (text[i] != '\0') {
        pToken = NULL;
        if (text[i] != '\n') {
            if (flag == 0) {
                pToken = strtok(array, "\n");
                newNode(pToken, new);
                flag++;
            } else {
                pToken = strtok(NULL, "\n");
                newNode(pToken, new);
            }
        i = i + strlen(pToken) + 1;
        } else if (newFlag == 0 && text[0] == '\n') {
            newNode("", new);
            newFlag++;
            i++;
        } else if (text[i - 1] == '\n' ) {
            newNode("", new);
            i++;        
        }
    }
    giveLineNum(new);      
    return new;
}




/**
 * Free  the  memory occupied by the given textbuffer. It is an error to
 * access the buffer afterwards.
 */
void releaseTB (TB tb) {
    if (tb == NULL) {
        fprintf(stderr, "empty buffer");
        abort();
    }
    if (tb->numOfNodes != 0){
        Node *curr = tb->first  ;  
        while (tb->first != NULL) {
            tb->first = tb->first->next;
            free(curr->string);
            free(curr);
            curr = tb->first;
        }
    }
    free(tb);

}

//finding the length of the required array for dump function
static int lengthOfDump (TB tb, bool showLineNumbers) {
    int length = 0;
    Node *curr = tb->first;
    int lineNumber = 1;
    while (curr != NULL) {
        if (showLineNumbers) {
            // number of digits = log10(lineNumber) + 1 as an int
            length = length + strlen(curr->string) + 4 + log10(lineNumber);         
        } else {
            length = length + strlen(curr->string) + 1;
        }
        lineNumber++;
        curr = curr->next;
    }
    length++;
    return length;    
}

/**
 * Allocate  and return a string containing all of the text in the given
 * textbuffer. If showLineNumbers is true, add a prefix corresponding to
 * the line number.
 */

char *dumpTB (TB tb, bool showLineNumbers) {
    if (tb->numOfNodes == 0) {
        char *c = "";
        return c;    
    }
    int length = lengthOfDump(tb, showLineNumbers);
    int lineNumber = 1;
    Node *curr = tb->first;
    char *newLine = "\n";
    // number of digits of n == log10(n) + 1
    char *new = malloc(sizeof(char) * length);
    strcpy(new, "");
    while (curr != NULL) {  
        if (!showLineNumbers) {
            strcat(new, curr->string);
            strcat(new, newLine);
        } else {
            sprintf(new + strlen(new), "%d. ", lineNumber); 
            strcat(new, curr->string);
            strcat(new, newLine);
        }
        lineNumber++;
        curr = curr->next;
    }
   
	return new;
}

/**
 * Return the number of lines of the given textbuffer.
 */
int linesTB (TB tb) {
	int number = 0;
	Node *curr = tb->first;
	while (curr != NULL) {
	    number++;
	    curr = curr->next;
	}
	
	return number;
}

/**
 * Add a given prefix to all lines between 'from' and 'to', inclusive.
 * - The  program  should abort() wih an error message if 'from' or 'to'
 *   is out of range. The first line of a textbuffer is at position 1.
 */
 

//finding the line with number from and return a point to it
static struct Node *findFrom(TB tb, int from) {
    Node *tmp = tb->first;
    if (from < 1) {
        from = 1;
    }
    while (tmp != NULL) {
        if (tmp->lineNum == from) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

//adding the appropriate prefix from the line number 'from' to line number 'to'
 void addPrefixTB (TB tb, int from, int to, char *prefix) {
    if (to < from || prefix == NULL) {
        abort();
    } else if (tb == NULL || strlen(prefix) == 0 || tb->first == NULL || tb->last->lineNum < from) {
        return;
    } else if (to > tb->last->lineNum) {
        to = tb->last->lineNum;
    } else if (to > tb->numOfNodes) {
        to = tb->numOfNodes;
    }
    Node *curr = findFrom(tb, from);
    
    int lengthOfPrefix = strlen(prefix) + 1;
    
    while (curr != NULL && curr->lineNum <= to) {
        int length = 0;
        length = length + lengthOfPrefix + strlen(curr->string);
        char *new = malloc(sizeof(char) * length);
        strcpy(new, prefix);
        strcat(new, curr->string);
        free(curr->string);
        curr->string = new;
        curr = curr->next;
    }
}

/**
 * Remove  the  lines between 'from' and 'to' (inclusive) from the given
 * textbuffer 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
 
//finding the line number with number to 
static Node *findTo (TB tb, int to) {
    struct Node *curr = tb->first;
    if (to > tb->last->lineNum) {
        to = tb->last->lineNum;
    }
    while (curr != NULL) {
        if (curr->lineNum == to) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

//delete a single node
static void deleteNode (Node *tmp) {
    tmp->next = NULL;
    tmp->prev = NULL;
    free(tmp->string);
    free(tmp);
}

//delete all the node 
static void deleteAllNode (TB tb) {
    tb->numOfNodes = 0;
    Node *curr = tb->first;
    Node *tmp = NULL;
    while (curr != NULL) {
        tmp = curr;
        curr = curr->next;
        deleteNode(tmp);  
    }
    tb->first = tb->last = NULL;
    
}

//delete only one node
static void deleteOneNode (TB tb, Node *start) {
    //only one node presents in the list
    tb->numOfNodes--;
    if (tb->numOfNodes == 1) {
        deleteNode(start);
        return;
    //in the front and we are sure that there are more than 1 node
    } else if (start == tb->first) {
        tb->first = tb->first->next;
        deleteNode(start);        
    //last place in the list
    } else if (start == tb->last) {
        tb->last = tb->last->prev;
        deleteNode(start);
    //somewhere in the middle and at least 3 nodes
    } else {
        start->prev->next = start->next;
        start->next->prev = start->prev;
        deleteNode(start);
    }
    
}

//delete the nodes between the line pointing start and end
static void deleteBetween (TB tb, Node *start, Node *end) {
    Node *n = end->next;
    Node *tmp = start;
    Node *curr = start->prev;
    while (start != n) {
        tmp = start;
        start = start->next;
        tb->numOfNodes--;
        deleteNode(tmp);        
    }
    curr->next = n;
    n->prev = curr;    
}

//deletion of linked list involving the first node
static void deleteFromStart (TB tb, Node *start, Node *end) {
    Node *p = end->next;
    while (start != p) {
        tb->numOfNodes--;
        start->next->prev = NULL;
        tb->first = start->next;
        deleteNode(start);
        start = tb->first;  
    }              
}                           

//deletion of linked list involving the last node                    
static void deleteToLast (TB tb, Node *start, Node *end) {
    Node *tmp = start->prev;
    while (start != NULL) {
        tb->numOfNodes--;
        start->next->prev = tmp;
        tmp->next = start->next;
        deleteNode(start);
        start = tmp->next;
        if (start->next == NULL) {
            deleteNode(start);
            tmp->next = NULL;
            return;
        }        
    }
    tb->last = tmp;
      
}

//deleting the lines between the line number from and to                            
 void deleteTB (TB tb, int from, int to) {
    if (tb->numOfNodes == 0 || tb->last->lineNum < from) {
        return;
    } else if (from < 1 && to < 1) {
        return;             
    } else if (to < from) {
        fprintf(stderr, "error\n");
        abort();                
    }
    Node *start = findFrom(tb, from);
    
    Node *end = findTo(tb, to); 
            
    //if deleting all the node in the list
    if (start == tb->first && end == tb->last) {
        deleteAllNode(tb);
    //if only deleting one node in the list
    } else if (start == end) {
        deleteOneNode(tb, start);
    //if only deleting in the middle of the list
    } else if (start != tb->first && end != tb->last) {
        deleteBetween(tb, start, end); 
    // deletion includes the first node        
    } else if (start == tb->first) {            
        deleteFromStart(tb, start, end);        
    // deletion includes the last node but not from the beginning
    } else if (end == tb->last) {
        deleteToLast(tb, start, end);    
    }   
}

//finding the line with the line number pos
static struct Node *findPos (TB tb, int pos) {
    Node *tmp = tb->first;
    while (tmp != NULL) {
        if (tmp->lineNum == pos) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return tmp;
}


/**
 * Merge 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the merged-in lines from 'tb2'.
 *   - 'tb2' can't be used anymore (as if we had used releaseTB() on it)
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 **/
void mergeTB (TB tb1, int pos, TB tb2) {
    if (tb1->last->lineNum + 1 < pos) {
        fprintf(stderr, "error\n");
        abort();
    } else if (tb2->numOfNodes == 0 || pos < 1) {
        return;
    }
   
    Node *curr = findPos(tb1, pos);
    tb1->numOfNodes += tb2->numOfNodes;
    //merging when the buffer is empty
    if (pos == 1 && tb1->numOfNodes == 0) {
        tb1->first = tb2->first;
        tb1->last = tb2->last;
        tb1->numOfNodes = tb2->numOfNodes;
    //merging at first line     
    } else if (curr == tb1->first) {
        tb1->first->prev = tb2->last;
        tb2->last->next = tb1->first;
        tb1->first = tb2->first;
    //merging after last line        
    } else if (curr == NULL) {
        tb1->last->next = tb2->first;
        tb2->first->prev = tb1->last;
        tb1->last = tb2->last;
    //merging before the last line
    } else {
        curr->prev->next = tb2->first;
        tb2->first->prev = curr->prev;
        curr->prev = tb2->last;
        tb2->last->next = curr;
    }
    free(tb2);

    giveLineNum(tb1);

}

//copying a given buffer and return a pointer to it
static TB copyEntireBuffer (TB tb2) {
    char *c = dumpTB(tb2, false);
    TB new = newTB(c);
    free(c);
    return new;
    
}


/**
 * Copy 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the pasted-in lines from 'tb2'.
 *   - 'tb2' is unmodified and remains usable independent of tb1.
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 **/
 
 void pasteTB (TB tb1, int pos, TB tb2) {
    TB new = copyEntireBuffer(tb2);
    mergeTB(tb1, pos, new);                  
}

//copying a part of the buffer
static TB copyBufferPart (TB tb, Node *start, Node *end) {
    TB new = createTB();
    
    while (start != NULL && start != end->next) {
        new->numOfNodes++;
        int length = strlen(start->string) + 1;
        //the initial case where there is no node in the linked list
        if (new->first == NULL) {
            new->first = new->last = malloc(sizeof(Node));
            new->first->string = malloc(sizeof(char) * length);
            strcpy(new->first->string, start->string);
            new->first->next = new->first->prev = NULL;
        } else {
            Node *tmp = new->last;
            tmp->next = malloc(sizeof(Node));
            tmp->next->string = malloc(sizeof(char) * length);
            strcpy(tmp->next->string, start->string);
            tmp->next->prev = tmp;
            tmp->next->next = NULL;
            new->last = tmp->next;                                
        }
        start = start->next;    
    }
    
    return new;
    
}

/**
 * Cut  the lines between and including 'from' and 'to' out of the given
 * textbuffer 'tb' into a new textbuffer.
 * - The result is a new textbuffer (much as one created with newTB()).
 * - The cut lines will be deleted from 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
TB cutTB (TB tb, int from, int to) {
    if (to < from) {
        return NULL;
    } else if (from < 1 || to > tb->last->lineNum) {
        fprintf(stderr, "error: outside range\n");
        abort();
    }
    Node *start = findPos(tb, from);
    Node *end = findPos(tb, to);
    TB tmp = copyBufferPart(tb, start, end);
    TB new = createTB();
    new->first = tmp->first;
    new->last = tmp->last;
    free(tmp);
    deleteTB(tb, from, to);
    

	return new;
}



//creating a new node of the matchNode linkedlist
static Match newMatchNode (Match pointer, int columnNum, int lineNum) {
    Match new = malloc(sizeof(struct _matchNode));
    new->next = NULL;
    new->columnNumber = columnNum;
    new->lineNumber = lineNum;
    if (pointer == NULL) {
        return new;
    }
    pointer->next = new;
    
    return new;    
}

//modify the tmp according to the searching key word
static void modifyTmp (char array[], char *point, char *search) {
    int index = (int)(point - array);
    int i = index;
    while (i < index + strlen(search)) {
        array[i] = '\n';
        i++;
    }    
}

/**
 * Return  a  linked list of match nodes containing the positions of all
 * of the matches of string 'search' in 'tb'.
 * - The textbuffer 'tb' should remain unmodified.
 * - The user is responsible for freeing the returned list.
 **/
  
Match searchTB (TB tb, char *search) {
    if (search == NULL) {
        abort();
    }
    int flag = 0;
    Node *curr = tb->first;
    //Match loop = Match head = NULL;
    Match head = NULL;
    Match loop = NULL;
    while (curr != NULL) {
        int length = strlen(curr->string) + 1;
        char tmp[length];
        strcpy(tmp, curr->string);
        char *point = strstr(tmp, search);
        while (point != NULL) {
            //the difference between where the current pointer is pointing to
            //and the pointer pointing to the first element of the array
            int columnNum = (int)(point - tmp) + 1;
            //cases where the matchNode linkedlist is empty
            if (flag == 0) {
                head = newMatchNode(loop, columnNum, curr->lineNum);
                loop = head;
                flag++;
            } else {
                newMatchNode(loop, columnNum, curr->lineNum);
                loop = loop->next;             
            }         
            //strcpy(tmp, c);a strcpy will always include the NULL terminator
            modifyTmp(tmp, point, search);
            point = strstr(tmp, search);
        
        }
        curr = curr->next;
    }
   
	return head;
	
}




/**
 * Search  every  line of the given textbuffer for every occurrence of a
 * set of specified substitutions and alter them accordingly.
 * - Refer to the spec for details.
 */
void formRichText (TB tb) {
   
}

////////////////////////////////////////////////////////////////////////
// Bonus challenges

char *diffTB (TB tb1, TB tb2) {
	return NULL;
}

void undoTB (TB tb) {

}

void redoTB (TB tb) {

}


