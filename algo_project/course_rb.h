//---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

//Code Author : Jun Su. Lim
//Student Num : 
//Header      : course_rb
//Created     : 16. 06. 02 
//Modified    : 16. 06. 04
//Description : course 정보를 red-black tree에 저장.
//            : rb_tree_iter_inorder와 rb_tree_destroy만 사용 요망.
//            : rb_tree_head->right가 red-black tree에 root.

//---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

#ifndef __COURSE_RB_H__
#define __COURSE_RB_H__
#include<malloc.h>
#include<stdio.h>
#define infoNIL 0
#define itemMIN -1
#define black 0
#define red 1
#define MAX_STACK_SIZE 1000
typedef int itemType;
typedef int infoType;

struct course
{
	char *course_no;//학수번호
	char *course_name;//강좌이름
	char *course_time;//개설시간
	int grade;//학점
};

struct node 
{
	itemType key; //record key : grade
	itemType tag; //color value
	struct course *p_course;//course를 가리키는 포인터
	struct node *left, *right;//트리 포인터
};

struct node* stack[MAX_STACK_SIZE];
int top;

struct node *rb_tree_head, *x, *p, *g, *gg;//red-black pointer
struct node *rb_tree_tail;
static void rb_tree_init();
static void rb_tree_split(itemType k);
static struct node* rb_tree_rotate(itemType k, struct node *y);
static void rb_tree_insert(struct course *t_course);
static void rb_tree_destroy(struct node *t_node);
static void node_destroy(struct node *t_node);
static void rb_tree_iter_inorder(struct node* root);

static void rb_tree_init()
{
	rb_tree_tail = (struct node*)malloc(sizeof(struct node));
	rb_tree_tail->left = rb_tree_tail;
	rb_tree_tail->right = rb_tree_tail;
	rb_tree_tail->tag = black;
	rb_tree_tail->p_course = NULL;
	rb_tree_head = (struct node*)malloc(sizeof(struct node));
	rb_tree_head->right = rb_tree_tail;
	rb_tree_head->tag = black; 
	rb_tree_head->p_course = NULL; 
	rb_tree_head->key = itemMIN;
}
static void rb_tree_split(itemType k)
{
	x->tag = red; x->left->tag = black; x->right->tag = black;
	if (p->tag) {
		g->tag = red;//recolor
		if (k<g->key != k<p->key) 
			p = rb_tree_rotate(k, g);//이중회전
		x = rb_tree_rotate(k, gg);//단일회전
		x->tag = black;
	}
}
static struct node* rb_tree_rotate(itemType k, struct node *y)
{
	//tree rotation
	struct node *high, *low;
	high = (k< y->key) ? y->left : y->right;
	if (k < high->key) { low = high->left; high->left = low->right; low->right = high; }//우회전
	else { low = high->right; high->right = low->left; low->left = high; }//좌회전
	if (k < y->key) y->left = low; 
	else y->right = low;
	
	return low;
}

static void rb_tree_insert(struct course *t_course) 
{
	int k = t_course->grade;
	x = rb_tree_head->right; 
	p = rb_tree_head; 
	g = rb_tree_head;
	while (x != rb_tree_tail) 
	{
		gg = g; g = p; p = x;
		x = (k < x->key) ? x->left : x->right;
		//좌우 자식 노드가 red이면 recolor
		if (x->left->tag == red && x->right->tag == red) rb_tree_split(k);
		
	}

	x = (struct node*)malloc(sizeof(struct node));
	x->left = rb_tree_tail;
	x->right = rb_tree_tail;
	x->p_course = t_course;
	x->tag = red; 
	x->key = t_course->grade;

	if (k < p->key) p->left = x; else p->right = x;
	//red-red를 방지하기 위해서 rb_tree_split 호출
	//루트 노드는 항상 black
	rb_tree_split(k); rb_tree_head->right->tag = black;
}


static void rb_tree_destroy(struct node *t_node)
{
	if (t_node == rb_tree_tail)
		return;

	rb_tree_destroy(t_node->left);
	rb_tree_destroy(t_node->right);
	node_destroy(t_node);
}

static void node_destroy(struct node *t_node)
{
	free(t_node->p_course->course_name);
	free(t_node->p_course->course_no);
	free(t_node->p_course->course_time);
	free(t_node->p_course);
	free(t_node);
}
static void push(struct node* node)
{
	if (top >= MAX_STACK_SIZE - 1)
	{
		perror("Stack Overflow");
		return;
	}

	top++;
	stack[top] = node;
}

static struct node* pop()
{
	struct node* node = NULL;
	if (top == -1)
		return rb_tree_tail;

	node = stack[top];
	top--;
	return node;
}

static void rb_tree_iter_inorder(struct node* root)
{
	struct node* ptr=root;
	top = -1;

	while (1)
	{
		while (ptr != rb_tree_tail)
		{
			push(ptr);
			ptr = ptr->left;
		}

		ptr = pop();
		if (ptr == rb_tree_tail) break;
		//-------------------------------------------------
		//이 부분이 트리를 순회하면서 course 정보를 출력하는 부분.
		//정보를 추출하기 위해서는 이 부분 사용하면 됨.
		printf("%s  %s  %s  %d\n", ptr->p_course->course_no, ptr->p_course->course_name, ptr->p_course->course_time, ptr->p_course->grade);
		//-------------------------------------------------
		ptr = ptr->right;
	}

}
//static void rb_tree_search(struct node *t_node) 
//{
//	//struct node *x = rb_tree_head->right;//rb_tree_head는 dummy node
//	
//	if (t_node == rb_tree_tail)
//		return;
//
//	rb_tree_search(t_node->left);
//	node_print(t_node);
//	rb_tree_search(t_node->right);
//
//}
//void node_print(struct node *t_node)
//{
//	if(t_node->p_course != NULL)
//		printf("%s %s\n", t_node->p_course->course_no, t_node->p_course->course_time);
//}
#endif
