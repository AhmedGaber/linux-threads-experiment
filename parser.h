#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

char **get_files_list();
char *read_line();
char* connect_strings(char *s1, char *s2);
char **parse_line(char *line);

#endif // PARSER_H_INCLUDED
