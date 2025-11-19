#pragma once

#include <atomic>
#include <memory>

template<typename T>
class lock_free_queue
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        std::shared_ptr<node> next;
        node() : next(nullptr) {}
    };

    std::shared_ptr<node> head;
    std::shared_ptr<node> tail;

    // Funciones auxiliares para operaciones atómicas sobre shared_ptr
    // (Necesarias pre-C++20. En C++20 usaríamos std::atomic<std::shared_ptr>)
    std::shared_ptr<node> atomic_load(const std::shared_ptr<node>& p) {
        return std::atomic_load(&p);
    }
    
    void atomic_store(std::shared_ptr<node>& p, std::shared_ptr<node> val) {
        std::atomic_store(&p, val);
    }

    bool atomic_cas(std::shared_ptr<node>& p, std::shared_ptr<node>& expected, std::shared_ptr<node> desired) {
        return std::atomic_compare_exchange_strong(&p, &expected, desired);
    }

public:
    lock_free_queue()
    {
        // Nodo dummy inicial
        std::shared_ptr<node> dummy(new node);
        atomic_store(head, dummy);
        atomic_store(tail, dummy);
    }

    lock_free_queue(const lock_free_queue& other) = delete;
    lock_free_queue& operator=(const lock_free_queue&) = delete;

    void push(T new_value)
    {
        std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
        std::shared_ptr<node> p(new node);
        p->data = new_data;

        while (true)
        {
            std::shared_ptr<node> t = atomic_load(tail);
            std::shared_ptr<node> next = atomic_load(t->next);

            if (t == atomic_load(tail)) // ¿Sigue siendo el tail actual?
            {
                if (next == nullptr) // ¿Era realmente el último nodo?
                {
                    // Intentar enlazar el nuevo nodo
                    if (atomic_cas(t->next, next, p))
                    {
                        // Éxito al enlazar. Ahora intentamos mover el tail (si falla, otro lo hará)
                        atomic_cas(tail, t, p);
                        return;
                    }
                }
                else
                {
                    // Tail no apuntaba al último, intentamos avanzarlo
                    atomic_cas(tail, t, next);
                }
            }
        }
    }

    bool try_pop(T& value)
    {
        while (true)
        {
            std::shared_ptr<node> h = atomic_load(head);
            std::shared_ptr<node> t = atomic_load(tail);
            std::shared_ptr<node> next = atomic_load(h->next);

            if (h == atomic_load(head)) // Consistencia
            {
                if (h == t) // ¿Cola vacía o tail desactualizado?
                {
                    if (next == nullptr)
                    {
                        return false; // Cola vacía
                    }
                    // Tail atrasado, intentamos avanzarlo
                    atomic_cas(tail, t, next);
                }
                else
                {
                    // Leer el valor antes de mover head
                    if (next) {
                        std::shared_ptr<T> res = next->data;
                        // Intentar mover head
                        if (atomic_cas(head, h, next))
                        {
                            value = *res;
                            return true;
                        }
                    }
                }
            }
        }
    }
    
    bool empty() {
        std::shared_ptr<node> h = atomic_load(head);
        std::shared_ptr<node> next = atomic_load(h->next);
        return next == nullptr;
    }
};