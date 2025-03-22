#ifndef CIRCULAR_DOUBLY_LINKED_LIST_H
#define CIRCULAR_DOUBLY_LINKED_LIST_H

/*
Código desenvolvido por: Cairo Alberto  
Estudante de Engenharia da Computação - PUC Goiás  
Repositório: https://github.com/CairoAlberto123  
Data de criação: 21/03/2025  
Descrição:  
Implementação de uma classe genérica de lista circular duplamente encadeada com funções de inserção, remoção, movimentação interna (move) e deslocamento de um nó para outra lista (deslocate).  
As funções implementadas incluem:
  - push (push(last), push(first), push(index, valor, side) e sobrecarga relativa a um valor de referência),
  - pop (pop(last), pop(first), popByValue, pop(index, side) e popAt),
  - move(index, newIndex): move um nó para outra posição na mesma lista,
  - insertAt(index, valor): insere um novo nó na posição especificada,
  - popAt(index): remove e retorna o valor do nó na posição especificada,
  - deslocate(source, dest, index, newIndex): desloca o nó do índice especificado de uma lista origem para uma posição específica de outra lista.  
Tecnologias utilizadas:  
Linguagem C++  
Licença: MIT  
2025 Cairo Alberto - Todos os direitos reservados.
*/

// Inclusão das bibliotecas necessárias
#include <iostream>     // Para operações de entrada e saída
#include <stdexcept>    // Para tratamento de exceções

using namespace std;

// Declaração do template para o nó da lista
template <typename T>
struct Node {
    T data;          // Dado armazenado no nó
    Node<T>* next;   // Ponteiro para o próximo nó da lista
    Node<T>* prev;   // Ponteiro para o nó anterior da lista

    // Construtor: inicializa o nó com o valor recebido e ponteiros nulos
    Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
};

// Classe genérica para lista circular duplamente encadeada
template <typename T>
class CircularDoublyLinkedList {
private:
    Node<T>* head;   // Ponteiro para o primeiro nó da lista
    int size;        // Armazena o número de elementos na lista

    // Método auxiliar para obter o nó na posição 'index'
    Node<T>* getNodeAt(int index) const {
        if (index < 0 || index >= size)
            throw out_of_range("Índice fora dos limites da lista.");
        Node<T>* current = head;
        // Percorre a lista até chegar no índice desejado
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current;
    }

    // Método auxiliar para buscar um nó que contenha o valor especificado
    Node<T>* getNodeByValue(const T& value) const {
        if (size == 0)
            return nullptr;
        Node<T>* current = head;
        // Percorre todos os nós procurando pelo valor
        for (int i = 0; i < size; i++) {
            if (current->data == value)
                return current;
            current = current->next;
        }
        return nullptr; // Retorna nullptr caso não encontre
    }

    // Método auxiliar para remover (desanexar) um nó específico sem retorno
    // Ajusta os ponteiros e deleta o nó
    void removeNode(Node<T>* node) {
        if (size == 0 || node == nullptr)
            throw runtime_error("Operação inválida: lista vazia ou nó nulo.");
        if (size == 1) { // Se houver apenas um elemento
            delete node;
            head = nullptr;
        } else {
            // Ajusta os ponteiros dos nós adjacentes
            node->prev->next = node->next;
            node->next->prev = node->prev;
            // Atualiza o head se o nó removido for o primeiro
            if (node == head)
                head = node->next;
            delete node;
        }
        size--;
    }

    // Método auxiliar para inserir um nó já criado em uma posição específica.
    // Não aloca nova memória, apenas posiciona o nó fornecido.
    void insertNodeAt(Node<T>* node, int index) {
        if (index < 0 || index > size)
            throw out_of_range("Índice fora dos limites da lista.");
        if (size == 0) { // Caso lista vazia
            head = node;
            node->next = node;
            node->prev = node;
        } else if (index == 0) { // Inserção no início
            node->next = head;
            node->prev = head->prev;
            head->prev->next = node;
            head->prev = node;
            head = node;
        } else if (index == size) { // Inserção no final
            Node<T>* tail = head->prev;
            tail->next = node;
            node->prev = tail;
            node->next = head;
            head->prev = node;
        } else { // Inserção em posição intermediária
            Node<T>* refNode = getNodeAt(index);
            node->prev = refNode->prev;
            node->next = refNode;
            refNode->prev->next = node;
            refNode->prev = node;
        }
        size++;
    }

public:
    // Construtor: inicializa a lista vazia
    CircularDoublyLinkedList() : head(nullptr), size(0) {}

    // Destrutor: remove todos os nós para liberar a memória
    ~CircularDoublyLinkedList() {
        while (size > 0) {
            pop(); // Remove sempre o último elemento
        }
    }

    // push(): insere um novo elemento no final da lista (push(last))
    void push(const T& value) {
        Node<T>* newNode = new Node<T>(value); // Cria um novo nó com o valor
        if (head == nullptr) { // Se a lista estiver vazia
            head = newNode;
            newNode->next = newNode;
            newNode->prev = newNode;
        } else {
            Node<T>* tail = head->prev; // Último nó da lista
            tail->next = newNode;
            newNode->prev = tail;
            newNode->next = head;
            head->prev = newNode;
        }
        size++;
    }

    // pushFirst(): insere um novo elemento no início da lista (push(first))
    void pushFirst(const T& value) {
        push(value);       // Insere o valor no final
        head = head->prev; // Atualiza o head para o nó recém-inserido
    }

    // push(const T& refValue, char side):
    // Busca o nó que contenha refValue e insere um novo nó à direita ('d') ou à esquerda ('r')
    // com o mesmo valor de referência
    void push(const T& refValue, char side) {
        Node<T>* refNode = getNodeByValue(refValue);
        if (refNode == nullptr)
            throw runtime_error("Valor de referência não encontrado na lista.");
        // Obtém o índice do nó de referência
        int idx = 0;
        Node<T>* current = head;
        while (idx < size && current != refNode) {
            current = current->next;
            idx++;
        }
        // Chama a sobrecarga para inserir à direita ou à esquerda
        push(idx, refValue, side);
    }

    // push(int index, const T& value, char side):
    // Insere um novo nó com 'value' à direita ('d') ou à esquerda ('r') do nó na posição index
    void push(int index, const T& value, char side) {
        if (index < 0 || index >= size)
            throw out_of_range("Índice fora dos limites da lista.");
        Node<T>* refNode = getNodeAt(index);
        Node<T>* newNode = new Node<T>(value);
        if (side == 'd') {
            // Inserção à direita: após o nó de referência
            newNode->next = refNode->next;
            newNode->prev = refNode;
            refNode->next->prev = newNode;
            refNode->next = newNode;
        } else if (side == 'r') {
            // Inserção à esquerda: antes do nó de referência
            newNode->prev = refNode->prev;
            newNode->next = refNode;
            refNode->prev->next = newNode;
            refNode->prev = newNode;
            // Atualiza o head se o nó de referência for o primeiro
            if (refNode == head)
                head = newNode;
        } else {
            delete newNode;
            throw invalid_argument("Parâmetro 'side' inválido. Use 'd' para direita ou 'r' para esquerda.");
        }
        size++;
    }

    // insertAt(int index, const T& value):
    // Insere um novo nó com 'value' na posição especificada.
    // Se index == 0 insere no início; se index == size insere no final.
    void insertAt(int index, const T& value) {
        if (index < 0 || index > size)
            throw out_of_range("Índice fora dos limites da lista.");
        if (index == 0) {
            pushFirst(value);
        } else if (index == size) {
            push(value);
        } else {
            Node<T>* newNode = new Node<T>(value);
            // Insere antes do nó que está na posição 'index'
            Node<T>* refNode = getNodeAt(index);
            newNode->prev = refNode->prev;
            newNode->next = refNode;
            refNode->prev->next = newNode;
            refNode->prev = newNode;
            size++;
        }
    }

    // pop(): remove o último elemento da lista (pop(last))
    void pop() {
        if (size == 0)
            throw underflow_error("Lista vazia. Não há elementos para remover.");
        if (size == 1) {
            delete head;
            head = nullptr;
        } else {
            Node<T>* tail = head->prev;
            tail->prev->next = head;
            head->prev = tail->prev;
            delete tail;
        }
        size--;
    }

    // popFirst(): remove o primeiro elemento da lista (pop(first))
    void popFirst() {
        if (size == 0)
            throw underflow_error("Lista vazia. Não há elementos para remover.");
        removeNode(head);
    }

    // popByValue(): remove o nó que contenha o valor especificado
    void popByValue(const T& value) {
        Node<T>* node = getNodeByValue(value);
        if (node == nullptr)
            throw runtime_error("Valor não encontrado na lista.");
        removeNode(node);
    }

    // popByValue(const T& value, char side):
    // Remove o nó à direita ('d') ou à esquerda ('r') do nó que contenha 'value'
    void popByValue(const T& value, char side) {
        Node<T>* refNode = getNodeByValue(value);
        if (refNode == nullptr)
            throw runtime_error("Valor de referência não encontrado na lista.");
        if (size < 2)
            throw runtime_error("Não é possível remover nó adjacente em uma lista com um único elemento.");
        if (side == 'd') {
            removeNode(refNode->next);
        } else if (side == 'r') {
            removeNode(refNode->prev);
        } else {
            throw invalid_argument("Parâmetro 'side' inválido. Use 'd' para direita ou 'r' para esquerda.");
        }
    }

    // pop(int index, char side):
    // Remove o nó à direita ('d') ou à esquerda ('r') do nó na posição index
    void pop(int index, char side) {
        if (size < 2)
            throw runtime_error("Não é possível remover nó adjacente em uma lista com um único elemento.");
        Node<T>* refNode = getNodeAt(index);
        if (side == 'd') {
            removeNode(refNode->next);
        } else if (side == 'r') {
            removeNode(refNode->prev);
        } else {
            throw invalid_argument("Parâmetro 'side' inválido. Use 'd' para direita ou 'r' para esquerda.");
        }
    }

    // popAt(int index):
    // Remove o nó na posição especificada e retorna o seu valor.
    T popAt(int index) {
        Node<T>* node = getNodeAt(index);
        T value = node->data;
        removeNode(node);
        return value;
    }

    // move(index, newIndex):
    // Move o nó da posição 'index' para a nova posição 'newIndex' dentro da mesma lista.
    void move(int index, int newIndex) {
        if (index < 0 || index >= size)
            throw out_of_range("Índice original fora dos limites da lista.");
        if (newIndex < 0 || newIndex > size - 1)
            throw out_of_range("Novo índice fora dos limites da lista.");
        if (index == newIndex)
            return; // Sem alteração se os índices forem iguais

        // Obtém o nó a ser movido
        Node<T>* movingNode = getNodeAt(index);
        // Desanexa o nó da lista sem deletá-lo
        movingNode->prev->next = movingNode->next;
        movingNode->next->prev = movingNode->prev;
        // Atualiza o head se necessário
        if (movingNode == head)
            head = movingNode->next;
        size--; // Reduz temporariamente o tamanho

        // Ajusta newIndex se o nó removido estava antes da nova posição
        if (index < newIndex)
            newIndex--;

        // Reinsere o nó na nova posição utilizando o método auxiliar
        insertNodeAt(movingNode, newIndex);
    }

    // Operador []: permite acesso via índice (leitura e escrita)
    T& operator[](int index) {
        Node<T>* node = getNodeAt(index);
        return node->data;
    }

    // get(): retorna o valor do elemento no índice especificado
    T get(int index) const {
        Node<T>* node = getNodeAt(index);
        return node->data;
    }

    // set(): define o valor do elemento no índice especificado
    void set(int index, const T& value) {
        Node<T>* node = getNodeAt(index);
        node->data = value;
    }

    // length(): retorna o número de elementos da lista
    int length() const {
        return size;
    }

    // print(index): imprime o elemento no índice especificado
    void print(int index) const {
        try {
            T value = get(index);
            cout << "Elemento no índice " << index << ": " << value << endl;
        } catch (const exception& e) {
            cout << "Erro ao imprimir: " << e.what() << endl;
        }
    }

    // printAll(): imprime todos os elementos da lista
    void printAll() const {
        if (size == 0) {
            cout << "Lista vazia." << endl;
            return;
        }
        Node<T>* current = head;
        cout << "Elementos da lista: ";
        // Percorre a lista e imprime os dados de cada nó
        for (int i = 0; i < size; i++) {
            cout << current->data;
            if (i < size - 1)
                cout << " -> ";
            current = current->next;
        }
        cout << endl;
    }
};

// Função global deslocate:
// Desloca o nó do índice 'index' da lista 'source' para a posição 'newIndex' na lista 'dest'
template <typename T>
void deslocate(CircularDoublyLinkedList<T>& source, CircularDoublyLinkedList<T>& dest, int index, int newIndex) {
    // Remove o nó da lista fonte e armazena seu dado
    T data = source.popAt(index);
    // Insere o dado na lista destino na posição especificada
    dest.insertAt(newIndex, data);
}

#endif // CIRCULAR_DOUBLY_LINKED_LIST_H
