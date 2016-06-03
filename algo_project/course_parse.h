//---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

//Code Author : Jun Su. Lim
//Student Num : 
//Header      : course_parse
//Created     : 16. 06. 02 
//Modified    : 16. 06. 04
//Description : 텍스트 문서를 파싱하는 함수를 가지는 헤더
//			  : parse_course 함수를 사용해서 텍스트 문서를 파싱
//            : parse_course 함수 호출 시에 파일 이름 전달.

//---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

#ifndef __COURSE_PARSE_H__
#define __COURSE_PARSE_H__
#define __CRT_SECURE_NO_WARNINGS
#include<string.h>
#include<stdlib.h>
#include"course_rb.h"//red-black tree
#define true 1
#define false 0


FILE *txt_file=NULL;
char next_start_string[] = "교과목영문명";

static void parse_course(char * txt_name)
{
	char str_buffer[100];
	char str_body_buffer[500];
	char *tok, *str;
	int ch, tok_count = 0;
	int i = 0, except_flag = false;
	struct course *p_course;

	txt_file = fopen(txt_name, "rt");
	if (txt_file == NULL) perror("Error opening file");

LOOP:
	fgets(str_buffer, 100, txt_file);

	tok = strtok(str_buffer, "\t");
	while (tok)
	{
		tok = strtok(NULL, "\t");
		if (tok == NULL) break;
		if (!strncmp(tok, next_start_string, strlen(next_start_string)))
			goto JUMP;
	}
	goto LOOP;

JUMP:
	//course pasing
	memset(str_body_buffer, 0, 500);
	rb_tree_init();

	while (1)
	{
		while ((ch = fgetc(txt_file)) != '\n')
		{
			if (ch == EOF)
				goto OUT;
			str_body_buffer[i++] = ch;
		}
		str_body_buffer[i] = '\0';

		tok = strtok(str_body_buffer, "\t");
		
		str = strstr(tok, "교양");
		if(str!=NULL)
			if (!strncmp(str, "교양", 2))
				++tok_count;

		p_course = (struct course*)malloc(sizeof(struct course));
		memset(p_course, 0, sizeof(struct course));
		
		while (tok)
		{
			if (!strncmp(tok, "DES3", 4) || !strncmp(tok, "RGC2007", 7) || !strncmp(tok, "EGC2048", 7) || !strncmp(tok, "EGC3037", 7))
			{
				while (tok)
					tok = strtok(NULL, "\t");
				except_flag = true;
				break;
			}


			if (++tok_count == 4)//학수번호
			{
				p_course->course_no = (char*)malloc((sizeof(char)*strlen(tok))+1);
				strcpy(p_course->course_no, tok);
			}
			else if (tok_count == 5)//강좌이름
			{
				p_course->course_name = (char*)malloc((sizeof(char)*strlen(tok))+1);
				strcpy(p_course->course_name, tok);
			}
			else if (tok_count == 8)//시간
			{
				p_course->course_time = (char*)malloc((sizeof(char)*strlen(tok))+1);
				strcpy(p_course->course_time, tok);
			}
			else if (tok_count == 10)//학점
				p_course->grade = atoi(tok);

			tok = strtok(NULL, "\t");

		}
		if(tok==NULL && except_flag==false)
			rb_tree_insert(p_course);
		else if (except_flag == true)
		{
			free(p_course);
			except_flag = false;
		}
		
		memset(str_body_buffer, 0, 500);
		tok_count = i = 0;
	}
	
OUT:
	rb_tree_iter_inorder(rb_tree_head->right);//트리 순회하면서 course 정보 출력

	rb_tree_destroy(rb_tree_head->right);//트리 파괴, 나중에 사용할 때는 위치 조정 필요
	fclose(txt_file);
}


#endif