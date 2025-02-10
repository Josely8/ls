#include "vector.h"
#include <stdlib.h>
#include <stddef.h>

// <-- YOUR CODE HERE -->

GenericVector* NewGenericVector(size_t capacity)
{
    GenericVector* gvec = malloc(sizeof(GenericVector));
    gvec->len_ = 0;
    gvec->capacity_ = capacity;
    gvec->arr_ = malloc(sizeof(void*) * capacity);
    return gvec;
}

void FreeGenericVector(GenericVector* vector) {
    if (!vector)
    {
        return;
    }
    for (int i = 0; i < vector->len_; ++i)
    {
        if(vector->arr_[i])
        {
            free(vector->arr_[i]);
        }
    }
    free(vector->arr_);
    free(vector);
}

void Append(GenericVector* vector, void* elem)
{
    if (vector->len_ == vector->capacity_)
    {
        vector->capacity_ += 5; 
        vector->arr_ = realloc(vector->arr_, vector->capacity_);
    }
    vector->arr_[vector->len_++] = elem;
}

/*void Extend(GenericVector* vector, GenericVector* source) {
    for (int i = 0; i < source->len_; ++i) {
        Append(vector, source->arr_[i]);
    }
    source->len_ = 0;
}*/

/*void* GetElement(const GenericVector* vector, size_t idx) {
    if (idx >= vector->len_) return NULL; //Примитивная обработка ошибки, мб написать че то можно :)
    return vector->arr_[idx];
}

void** GetData(const GenericVector* vector) {
    return vector->arr_;
}*/

size_t GetLength(const GenericVector* vector) {
    return vector->len_;
}

/*bool IsEmpty(const GenericVector* vector) {
    return (vector->len_ == 0);
}*/
