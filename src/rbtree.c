#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  // *t 포인터의 변수이름
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree)); // 트리 생성
  node_t *nil_node = (node_t*)calloc(1, sizeof(node_t)); //nil노드 생성
  nil_node->color = RBTREE_BLACK; //모든 리프 노드는 검정
  t->nil = nil_node;  //부모 노드도 nil을 가르켜야 함
  t->root = nil_node; // root노드 초기화
  return t; //  새로 생성된 트리의 메모리 주소를 반환, 
  //다른 함수나 코드에서 이 주소를 통해 새 트리에 접근
}


// 노드 메모리 해제
void delete_node(node_t *node, rbtree *t){
  // 현재 노드가 nil이면 return : 아무 값이 없어서 할당될것이 없음
  if (node == t->nil){
    return;
  }
  // 재귀 호출로 끝까지 간 후 해제
  delete_node(node->left, t);
  delete_node(node->right, t);
  free(node); // 현재 노드가 가리키는 메모리를 해제
  node = NULL; // 현재 노드 포인터를 NULL로 설정, 해제된 메모리에 접근하는 것을 방지
  return;
}


// 트리 메모리 해제
void delete_rbtree(rbtree *t) {
  // 트리가 NULL인 경우, 잘못된 메모리 접근을 방지
  if (t == NULL)
    return;

  delete_node(t->root, t); //  트리의 루트 노드를 시작으로 모든 노드를 메모리에서 해제
  free(t->nil);  // nil 노드를 메모리에서 해제
  t->nil = NULL;  // 할당 해제 후 nil값 null로 초기화 : 해제된 노드를 가리키는 포인터를 사용하지 않도록 함. 해제된 메모리에 접근하는 것을 방지
  free(t); // 트리 자체를 가리키는 메모리를 해제
  t = NULL; // 트리 포인터를 NULL로 설정
  return; // 함수 종료
}


// 트리 t와 그 안의 특정 노드 x를 인자로 받아, x를 중심으로 왼쪽 회전을 수행
void left_rotation(rbtree *t, node_t *x){
  node_t *y = x->right;  //x의 오른쪽 자식 노드를 y로 설정
  x->right = y->left;  // y의 왼쪽 자식을 x의 오른쪽 자식으로 설정

  if (y->left != t->nil){  // y의 왼쪽 자식이 존재한다면
    y->left->parent = x;  // 그 노드의 부모를 x로 설정
  }

  y->parent = x->parent;  // x의 부모를 y의 부모로 설정(좌회전)

  //  y가 x의 위치로 올라가므로 y를 x의 부모와 연결하는 역할
  if (x->parent == t->nil){  // x의 부모가 nil이라면 (루트라면)
    t->root = y; // y를 트리의 루트로 설정
  }
  else if ( x == x->parent->left){ // x가 부모의 왼쪽 자식이라면
    x->parent->left = y;  // y를 그 부모의 왼쪽 자식으로 설정
  }
  else {  // x가 오른쪽 자식이라면
    x->parent->right = y;  // y를 그 부모의 오른쪽 자식으로 설정
  }

  y->left = x;  // y의 왼쪽 자식을 x로 설정, y가 x의 위치로 올라가므로, x는 y의 왼쪽 자식
  x->parent = y;  // x의 부모를 y로 설정, y가 x의 위치로 올라가므로, x의 부모는 y
  return;
}


// 트리 t와 그 안의 특정 노드 x를 인자로 받아, x를 중심으로 오른쪽 회전을 수행
void right_rotation(rbtree *t, node_t *x){
  node_t *y = x->left;  // x의 왼쪽 자식 노드를 y로 설정
  x->left = y->right;  //y의 오른쪽 자식을 x의 왼쪽 자식으로 설정

  if (y->right != t->nil){ // y의 오른쪽 자식이 존재한다면
    y->right->parent = x; // 그 노드의 부모를 x로 설정
  }
  y->parent = x->parent;  // x의 부모를 y의 부모로 설정 (우회전시킴)

  // y가 x의 위치로 올라가므로 y를 x의 부모와 연결하는 역할
  if (x->parent == t->nil){  // x의 부모가 nil이라면, (루트라면)
    t->root = y; // y를 트리의 루트로 설정
  }
  else if ( x == x->parent->left){ // x가 부모의 왼쪽 자식이라면
    x->parent->left = y; // y를 그 부모의 왼쪽 자식으로 설정
  }
  else { // x가 부모의 오른쪽 자식이라면
    x->parent->right = y; // y를 그 부모의 오른쪽 자식으로 설정
  }
  
  y->right = x; //  y의 오른쪽 자식을 x로 설정, y가 x의 위치로 올라가므로, x는 y의 오른쪽 자식
  x->parent = y; // x의 부모를 y로 설정, y가 x의 위치로 올라가므로, x의 부모는 y
  return;
}

// 새 노드를 추가한 후 rbtree 속성을 유지하기 위해 필요한 조정하는 함수
void rbtree_insert_fixup(rbtree *t, node_t *z) { // 트리 t, 새로 추가된 노드 z
  while (z->parent->color == RBTREE_RED)  {  // z의 부모 노드가 빨간색인 경우 (double red)

    // z의 부모 노드가 조부모 노드의 왼쪽 자식인 경우와 오른쪽 자식인 경우를 구분하여 처리
    // 회전의 방향을 결정하기 위함
    if (z->parent == z->parent->parent->left) {
      node_t *uncle = z->parent->parent->right; // z의 삼촌 노드를 설정
      // CASE 1 = 부모 R + 삼촌 R
      // 삼촌 노드의 색이 빨간색인 경우와 검정색인 경우를 구분하여 처리
      if (uncle->color == RBTREE_RED) {     // 삼촌 레드일 때
        // z의 부모 노드와 삼촌 노드의 색을 검정색으로, 조부모 노드의 색을 빨간색으로 변경
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;    // 조상은 무조건 red, red여야 while문 돈다!
      }
      else if (uncle->color == RBTREE_BLACK) {    // 삼촌 블랙일 때
          // CASE 2-(1) = 꺾새 모양일 때
          if (z == z->parent->right) {            // 타켓이 오른쪽 자식이면
            z = z->parent;                        // 타켓을 그 부모로 바꾸고
            left_rotation(t, z);                  // left 로테이션 고고
          }
          // CASE 2-(2) = 일렬일 때
          z->parent->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          right_rotation(t, z->parent->parent);   // 조상 기준 우회전
      }
    }
    // z의 부모가 z조상의 오른쪽이면 위의 과정 반대로 하면 됨
    else if (z->parent==z->parent->parent->right) {
      node_t *uncle = z->parent->parent->left;
      // CASE 1 = 부모 R + 삼촌 R
      if (uncle->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else if (uncle->color == RBTREE_BLACK) {
        // CASE 2-(1) = 꺾새 모양일 때
        if (z == z->parent->left) {
          z = z->parent;
          right_rotation(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotation(t, z->parent->parent);
      }
    }
  } // while문 종료
  t->root->color = RBTREE_BLACK;    // 트리의 루트 노드의 색을 검정색으로 설정
  return;   // void형
} 
// rbtree에 새 노드를 삽입하는 함수
node_t *rbtree_insert(rbtree *t, const key_t key) { 
  // 삽입 위치를 찾기 위한 노드 x를 트리의 루트
  node_t *x = t->root;
  // x의 부모 노드 y를 nil로 초기화
  node_t *y = t->nil;


  // x가 리프노드에 도달할 때까지 반복
  while (x != t->nil){
    // tmp역할 (nil을 만나기 직전노드의 값에 z를 넣기위해 y에 저장)
    y = x;
    // 만약 x의 key값보다 삽입할 key값이 작으면
    if (key < x->key){ 
    //  x를 x의 왼쪽으로 변경
      x = x->left;
    }
    // 만약 x의 key값보다 삽입할 key값이 크거나 같으면
    else{
    // x를 x의 오른쪽으로 변경
      x = x->right;
    }
  }
  // while문 종료 = x가 nil을 가리킴 -> new node(z)가 삽입할 시기
  // z노드 생성
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key; // z의 key갑을 넣어줌
  z->parent = y; // z의 부모는 미리 저장해놓은 y
  // y의 트리의 nil = 비어있던 애
  if (y == t->nil){
    t->root = z; // 루트는 z
  }
  // y의 key값이 z의 key값보다 크다면
  else if (z->key < y->key){
    y->left = z; // z는 y의 왼쪽 자식
  }
  // y의 key값이 z의 key값보다 작거나 같으면
  else {
    y->right = z; // z는 y의 오른쪽 자식
  }
  
  z->left = t->nil; // z의 왼쪽 자식 nil
  z->right = t->nil; // z의 오른쪽 자식 nil
  z->color = RBTREE_RED; // new_node는 늘 red

  // 새 노드 z를 삽입한 후 rbtree 속성을 유지하기 위해 필요한 조정 작업을 수행
  rbtree_insert_fixup(t, z);
  // 삽입 연산 후에 트리의 루트 노드를 반환
  return t->root;
}


// rbtree에서 주어진 키를 가진 노드를 찾는 함수
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *x = t->root; // 노드 x를 트리의 루트로 초기화, 트리를 탐색하는 데 사용
  // x가 nil이 아니고 x의 키가 찾는 키와 다른 동안 트리를 탐색, 이 과정을 통해 찾는 키를 가진 노드를 찾는다. 
  while (x != t->nil && key != x->key){
    if (key < x->key) // key가 x의 키보다 작으면 x를 왼쪽 자식
      x = x->left;
    else              // key가 x의 키보다 크면 x를 오른쪽 자식
      x = x->right; 
  }
  // x가 nil이면 트리에 없으므로 NULL을 반환
  if (x == t->nil)
    return NULL;
  // 찾는 키를 가진 노드 x를 반환
  return x;
}


// 트리의 최소값
node_t *rbtree_min(const rbtree *t) {
  node_t *x = t->root; // 노드 x를 트리의 루트로 초기화, x는 트리를 탐색하는 데 사용
  while(x->left != t->nil){ // x의 왼쪽 자식이 nil이 아닌 동안 x를 왼쪽 자식으로 변경
    x = x->left;
  }
  return x; // 가장 작은 키를 가진 노드 x를 반환
}


// 트리의 최대값
node_t *rbtree_max(const rbtree *t) {
  node_t *x = t->root;
  while(x->right != t->nil){
    x = x->right;
  }
  return x;
}


// successor를 찾는 함수
node_t *find_successor(rbtree *t, node_t *x){
  node_t *succ = x;   // x의 후계자
  while (succ->left != t->nil) { 
  // x의 왼쪽 자식 노드를 계속 탐색하여
  // x의 서브트리에서 가장 작은 키를 가진 노드를 찾습니다.
    succ = succ->left;
  }
  return succ;
}

// 특정 노드를 삭제하고 그 자리에 successor를 붙이는 함수
void rb_transplant(rbtree *t, node_t *target, node_t *succ){
  if (target->parent == t->nil) // 만약 target이 루트 노드라면
    t->root = succ; // succ를 새로운 루트로 설정
  else if (target == target->parent->left) // 만약 target이 부모 노드의 왼쪽 자식이라면
    target->parent->left = succ; //  target의 부모 노드의 왼쪽 자식을 succ로 설정
  else  // 만약 target이 부모 노드의 오른쪽 자식이라면
    target->parent->right = succ; // target의 부모 노드의 오른쪽 자식을 succ로 설정
  succ->parent = target->parent; // succ의 부모 노드를 target의 부모 노드로 설정, target이 원래 있던 위치에 succ가 이식
  return;
}


// rbtree에서 노드를 삭제한 후 트리의 균형을 재조정하는 함수
void rbtree_erase_fixup(rbtree *t, node_t *x){
  node_t *w;
  // x가 루트 노드가 아니고 x의 색깔이 검정인 동안 계속 실행
  while (x != t->root && x->color == RBTREE_BLACK){
    if(x == x->parent->left){ // x가 부모 노드의 왼쪽 자식일 때
      w = x->parent->right;
      // CASE 1: w 레드
      if(w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotation(t, x->parent);
        w = x->parent->right; 
      }
      // CASE 2: w 블랙, w의 두 자녀 블랙
      if ((w->left->color == RBTREE_BLACK)&&(w->right->color == RBTREE_BLACK)){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      // CASE 3: 꺾새 모양 w의 왼쪽 자녀 red, 오른쪽 자녀 black
      else {
        if (w->right->color == RBTREE_BLACK){
          w->left->color = RBTREE_BLACK;
          w->color =RBTREE_RED;
          right_rotation(t, w);
          w = x->parent->right;
        }
        // CASE 4: 일렬 (최종)
        w->color = x->parent->color;
        w->right->color = RBTREE_BLACK;
        x->parent->color = RBTREE_BLACK;
        left_rotation(t, x->parent);
        x = t->root; // while 종료 조건
      }      
    }
    // x가 부모 노드의 오른쪽 자식일 때
    else if(x == x->parent->right){
      w = x->parent->left;
      // CASE 1: w 레드
      if(w->color ==RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotation(t, x->parent);
        w = x->parent->left; 
      }
      // CASE 2: w 블랙, w의 두 자녀 블랙
      if ((w->left->color == RBTREE_BLACK)&&(w->right->color == RBTREE_BLACK)){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      // CASE 3: 꺾새 모양 w의 오른쪽 자녀 red, 왼쪽자녀 black
      else {
        if (w->left->color == RBTREE_BLACK){
          w->right->color = RBTREE_BLACK;
          w->color =RBTREE_RED;
          left_rotation(t, w);
          w = x->parent->left;
          }
        // CASE 4: 일렬 (최종)
        w->color = x->parent->color;
        w->left->color = RBTREE_BLACK;
        x->parent->color = RBTREE_BLACK;
        right_rotation(t, x->parent);
        x = t->root;
      }      
    }
  }
  x->color = RBTREE_BLACK; // x의 색깔을 검정으로 변경, #5규칙을 만족시키기 위함
  return;
}


// rbtree에서 특정 노드를 삭제하는 함수
int rbtree_erase(rbtree *t, node_t *target) {
  node_t *p = target; // 삭제할 노드를 p에 복사하여 포인터처럼 활용
  color_t target_original_color = p->color; // 지워질 노드의 색을 변수에 저장
  node_t *x; // succ의 값의 자리를 저장해두는 포인터, fixup의 기준이 될 노드

  // 만약 삭제할 노드의 왼쪽 자식이 없는 경우
  if (target->left == t->nil){ 
    // x에 삭제할 노드의 오른쪽 자식을 저장하고, 삭제할 노드와 그 오른쪽 자식을 이식
    x = target->right; 
    rb_transplant(t, target, target->right);
  }
  // 만약 삭제할 노드의 오른쪽 자식이 없는 경우
  else if (target->right == t->nil){
    // x에 삭제할 노드의 왼쪽 자식을 저장하고, 삭제할 노드와 그 왼쪽 자식을 이식
    x = target->left;
    rb_transplant(t, target, target->left);
  }
  // 삭제할 노드가 양쪽 자식을 모두 가진 경우
  else{
    // 삭제할 노드의 후계자를 찾아 이식하고, 필요에 따라 트리를 재조정
    p = find_successor(t, target->right);
    target_original_color = p->color;
    x = p->right;

    if (p->parent == target){
      x->parent = p;
    }
    else{
      rb_transplant(t, p, p->right);
      p->right = target->right;
      p->right->parent = p;
    }
    rb_transplant(t, target, p);
    p->left = target->left;
    p->left->parent = p;
    p->color = target->color;
  }

  free(target); // 삭제한 노드의 메모리를 해제
  target = NULL; // 누수를 방지하기 위해 포인터를 NULL로 설정

  // 삭제한 노드의 색이 검정색이라면 트리의 균형을 재조정
  if (target_original_color == RBTREE_BLACK){
    rbtree_erase_fixup(t, x);
  }
  return 0;
}


// rbtree의 노드를 중위 순회(inorder traversal)하면서 각 노드의 키 값을 배열에 저장하는 함수
int rbtree_inorder(node_t *nil, node_t *root, key_t *arr, const size_t n, int index){
  // 현재 노드가 nil인 경우 
  if (root == nil)
  // 재귀를 종료하고 현재 인덱스를 반환
    return index;
  
  // 배열의 크기 n까지 순회했다면
  if (index == n)
  // 재귀를 종료하고 현재 인덱스를 반환 (배열의 범위를 초과하지 않기 위한 조건)
    return index;
  
  // 왼쪽 자식 노드를 중위 순회, 재귀 호출 결과 반환된 인덱스를 현재 인덱스로 업데이트
  index = rbtree_inorder(nil, root->left, arr, n, index); 

  // 배열의 범위를 초과하지 않는 한, 현재 노드의 키 값을 배열에 저장하고 인덱스를 1 증가
  if (index < n)
    arr[index++] = root->key;   // arr[현재 인덱스] = 현재 노드의 key값

  // 오른쪽 자식 노드를 중위 순회, 재귀 호출 결과 반환된 인덱스를 현재 인덱스로 업데이트
  index = rbtree_inorder(nil, root->right, arr, n, index); 

  // 마지막으로 반환된 인덱스를 반환, 다음 노드의 키 값을 저장할 배열의 인덱스로 사용
  return index;
}



// rbtree에 있는 모든 노드의 키 값을 오름차순으로 배열에 저장하는 함수
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // 트리의 루트 노드가 nil인 경우(트리가 비어 있는 경우)
  if (t->root == t->nil) 
  // 함수 종료   
    return 0;
  
  // 트리를 중위 순회하여 각 노드의 키 값을 배열에 저장
  // 왼쪽 자식 노드 -> 부모 노드 -> 오른쪽 자식 노드 순서로 노드를 방문(오름차순으로 방문)
  rbtree_inorder(t->nil, t->root, arr, n, 0);  
  return 0;
}
