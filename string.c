#include "tree.c"

typedef struct splay_string
{
    splay_tree * tree;
    const unsigned int nodes_count;
} splay_string;

typedef struct string_element
{
    unsigned int index;
    char * element;
} string_element;

bool bigger_predicate(void * first_element, void * second_element)
{
    return ((string_element *)(first_element))->index > ((string_element *)(second_element))->index;
}

bool equal_predicate(void * first_element, void * second_element)
{
    return ((string_element *)(first_element))->index == ((string_element *)(second_element))->index;
}

string_element * new_string_element(unsigned int index, char * element)
{
    string_element * str_element = malloc(sizeof(*str_element));
    str_element->element = element;
    str_element->index = index;
    return str_element;
}

unsigned int strlen(char * string)
{
    unsigned int length = 0;
    char el = string[length];
    while (el != '\0')
    {
        ++length;
        el = string[length];
    }
    return length;
}

splay_string * new_string(char * char_string)
{
    splay_string * string = malloc(sizeof(*string));
    *(unsigned int *)&string->nodes_count = strlen(char_string);
    string->tree = new_tree();
    for (unsigned int i = 0; i < string->nodes_count; ++i)
    {
        string_element * element = new_string_element(i, &char_string[i]);
        insert(string->tree, element, bigger_predicate);
    }
    return string;
}

void set_indices(unsigned int * start_index, unsigned int * last_index, splay_string * string)
{
    *start_index = 0;
    *last_index = string->nodes_count;
}

unsigned int find_substring(splay_string * string, splay_string * substring, unsigned int start_index, unsigned int end_index)
{
   set_indices(&start_index, &end_index, string); 
   unsigned int substring_length = substring->nodes_count;
   for (unsigned int i = 0; i < end_index - substring_length + 1; ++i)
   {
       unsigned int j = 0;
       while (j < substring_length)
       {
           string_element * str_element = new_string_element(i + j, NULL);
           splay_node * str_node = _search(string->tree, (void *)(str_element), bigger_predicate, equal_predicate);
           string_element * substr_element = new_string_element(j, NULL);
           splay_node * substr_node = _search(substring->tree, (void *)(substr_element), bigger_predicate, equal_predicate);
           char * char_str_el = ((string_element *)(str_node->data))->element;
           char * char_substr_el = ((string_element *)(substr_node->data))->element;
           if (*char_str_el != *char_substr_el)
           {
               break;
           }
           ++j;
       }
       if (j == substring_length)
       {
           return i;
       }
   }
   unsigned int not_found_code = -1;     
   return not_found_code;
}

unsigned int main(){
    char * string = "Hello!";
    char * substring = "!";
    splay_string * string_ = new_string(string);
    splay_string * substring_ = new_string(substring);
    unsigned int start, end;
    printf("%d", find_substring(string_, substring_, start, end));
    return 0;
}
