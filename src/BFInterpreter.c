#include <stdio.h>
#include <stdlib.h>

#define NULL ((void *)0)

typedef struct bfnode bfnode;
struct bfnode
{
	bfnode *fwd, *bck;
	int value;
};

// node initializer
bfnode* newNode(bfnode *, bfnode *);

// node destroyer
void destroyNode(bfnode *);

// ptr movers
bfnode* dec(bfnode *ptr);
bfnode* inc(bfnode *ptr);

// processors
int run(char *);
char* runSubLoop(char *, bfnode *);

// oh shit function
void bfabort(bfnode *);

int main(int argc, char **argv)
{
	if (argc > 1) return run(argv[1]);

	return 0;
}

int run(char *input)
{
	if (input == NULL) return 0;

	// initialize tape
	bfnode *ptr = newNode(NULL, NULL);
	
	while (*input != '\0')
	{
		switch (*input)
		{
			case '+':
				ptr->value++;
				break;
			case '-':
				ptr->value--;
				break;
			case '<':
				ptr = dec(ptr);
				break;
			case '>':
				ptr = inc(ptr);
				break;
			case '[':
				input = runSubLoop(input, ptr);
				break;
			case ']':
				bfabort(ptr);
				return 1;
			case '.':
				putchar(ptr->value);
				break;
		}

		input++;
	}

	// destroy tape
	while ((ptr != NULL) && (ptr->fwd != NULL)) destroyNode(ptr->fwd);
	while ((ptr != NULL) && (ptr->bck != NULL)) destroyNode(ptr->bck);
	destroyNode(ptr);

	printf("\n");

	return 0;
}

char* runSubLoop(char *input, bfnode *ptr)
{
	char *start = input;
	char *end = NULL;

	while ((*input != '[') || (ptr->value != 0))
	{
		input++;
		if (*input == '\0')
		{
			bfabort(ptr);
		}
		switch (*input)
		{
			case '+':
				ptr->value++;
				break;
			case '-':
				ptr->value--;
				break;
			case '<':
				ptr = dec(ptr);
				break;
			case '>':
				ptr = inc(ptr);
				break;
			case '[':
				input = runSubLoop(input, ptr);
				break;
			case ']':
				if (end == NULL) end = input;
				input = start;
				break;
			case '.':
				putchar(ptr->value);
				break;
		}
	}

	int loopCount = 0;

	if (end == NULL)
	{
		while ((loopCount > 0) || (*input != ']'))
		{
			input++;
			if (*input == '[') loopCount++;
			if (*input == ']') loopCount--;
		}
	}
	else
	{
		input = end;
	}

	return input;
}	

bfnode* dec(bfnode *ptr)
{
	if (ptr == NULL) return NULL;

	if (ptr->bck == NULL) ptr->bck = newNode(NULL, ptr);

	return ptr->bck;
}

bfnode* inc(bfnode *ptr)
{
	if (ptr == NULL) return NULL;

	if (ptr->fwd == NULL) ptr->fwd = newNode(ptr, NULL);

	return ptr->fwd;
}

bfnode* newNode(bfnode *b, bfnode *f)
{
	bfnode *node = malloc(sizeof(*node));
	
	if (node != NULL)
	{
		node->fwd = f;
		node->bck = b;
		node->value = 0;
	}

	return node;
}

void destroyNode(bfnode *node)
{
	if (node != NULL)
	{
		if (node->fwd != NULL)
		{
			node->fwd->bck = node->bck;
		}
		if (node->bck != NULL)
		{
			node->bck->fwd = node->fwd;
		}

		node->fwd = NULL;
		node->bck = NULL;
		free(node);
	}
}

void bfabort(bfnode *ptr)
{
	if (ptr == NULL)
	{
		printf("ptr is NULL.\n");
		return;
	}

	while (ptr->bck != NULL) ptr = ptr->bck;

	printf("Dumping tape contents:\n");
	while (ptr != NULL)
	{
		printf("%d ", ptr->value);
		if (ptr->fwd == NULL)
		{
			destroyNode(ptr);
			ptr = NULL;
		}
		else
		{
			ptr = ptr->fwd;
			destroyNode(ptr->bck);
		}
	}

	printf("\n");
}

