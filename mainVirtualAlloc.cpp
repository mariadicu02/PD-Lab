#include <windows.h>
#include <stdio.h>

int main() {
    SIZE_T chunkSize;       
    void* allocatedMemory;

    
    printf("Introduceti dimensiunea chunkului de memorie (in octeti): ");
    if (scanf("%zu", &chunkSize) != 1) {
        printf("Eroare la citirea dimensiunii!\n");
        return 1;
    }

 
    allocatedMemory = VirtualAlloc(
        NULL,                  
        chunkSize,             
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE        
    );

  
    if (allocatedMemory == NULL) {
        printf("Eroare la alocarea memoriei!\n");
        return 1;
    }

 
    if (memset(allocatedMemory, 0, chunkSize) == NULL) {
        printf("Eroare la setarea memoriei!\n");
        VirtualFree(allocatedMemory, 0, MEM_RELEASE);
        return 1;
    }

    printf("Memoria a fost alocata si setata la 0.\n");

  
    printf("Memoria a fost alocata la adresa: %p\n", allocatedMemory);


    if (!VirtualFree(allocatedMemory, 0, MEM_RELEASE)) {
        printf("Eroare la eliberarea memoriei!\n");
        return 1;
    }

    printf("Memoria a fost eliberata.\n");
    return 0;
}
