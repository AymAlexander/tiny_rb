/******************************************************************************/
/**
 * \file  tiny_rb.h
 *
 * \brief Lightweight generic ring buffer implementation (non-thread-safe)
 *        Users must add locking mechanisms if needed in concurrent contexts.
 *
 * \author    Aym <aymhzy@163.com>
 * \date      2026-01-09
 * \version   1.0.0
 *
 * \copyright
 * Copyright 2026
 *
 * The License of this software follows MIT, See the LICENSE for more details:
 * https://opensource.org/license/mit
 */

#ifndef __TINY_RB_H__
#define __TINY_RB_H__

#include <stddef.h>
#include <string.h>

/**
 * \brief   Declare a static FIFO ring buffer
 *
 * \param   [in] TYPE      Data type of the buffer elements
 * \param   [in] NAME      Name of the buffer (used in macro expansions)
 * \param   [in] CAPACITY  Maximum capacity of the buffer
 */
#define TRB_RB_DEFINE(TYPE, NAME, CAPACITY)\
static struct {\
    TYPE   buf[CAPACITY];\
    size_t capacity;\
    size_t head;\
    size_t tail;\
    size_t count;\
} _trb_##NAME##_buf = {\
    .buf = {0},\
    .capacity = CAPACITY,\
    .head = 0,\
    .tail = 0,\
    .count = 0\
}

/**
 * \brief   Check if the buffer is empty
 *
 * \param   [in] NAME      Buffer name
 *
 * \return  - (1): Empty
 *          - (0): Not empty
 */
#define trb_is_empty(NAME)\
    (_trb_##NAME##_buf.count == 0)

/**
 * \brief   Check if the buffer is full
 *
 * \param   [in] NAME      Buffer name
 *
 * \return  - (1): Full
 *          - (0): Not full
 */
#define trb_is_full(NAME)\
    (_trb_##NAME##_buf.count == _trb_##NAME##_buf.capacity)

/**
 * \brief   Get the number of elements currently in the buffer
 *
 * \param   [in] NAME      Buffer name
 *
 * \return  Number of elements (size_t)
 */
#define trb_size(NAME)\
    (_trb_##NAME##_buf.count)

/**
 * \brief   Get the total capacity of the buffer
 *
 * \param   [in] NAME      Buffer name
 *
 * \return  Total capacity (size_t)
 */
#define trb_capacity(NAME)\
    (_trb_##NAME##_buf.capacity)

/**
 * \brief   Get the remaining free space in the buffer
 *
 * \param   [in] NAME      Buffer name
 *
 * \return  Number of free slots (size_t)
 */
#define trb_remaining(NAME)\
    (_trb_##NAME##_buf.capacity - _trb_##NAME##_buf.count)

/**
 * \brief   Clear the buffer (reset to empty state)
 *
 * \param   [in] NAME      Buffer name
 */
#define trb_flush(NAME)\
    do {\
        _trb_##NAME##_buf.head = 0;\
        _trb_##NAME##_buf.tail = 0;\
        _trb_##NAME##_buf.count = 0;\
    } while (0)

/**
 * \brief   Push an element into the buffer
 *
 * \param   [in] NAME      Buffer name
 * \param   [in] VALUE_PTR Pointer to the element to be pushed
 *
 * \return  - (0)  Success
 *          - (-1) Buffer full
 */
#define trb_fifo_push(NAME, VALUE_PTR)\
    (trb_is_full(NAME)?(-1):\
    (memcpy(&_trb_##NAME##_buf.buf[_trb_##NAME##_buf.tail], VALUE_PTR, sizeof(_trb_##NAME##_buf.buf[0])),\
    _trb_##NAME##_buf.tail = (_trb_##NAME##_buf.tail + 1) % _trb_##NAME##_buf.capacity,\
    _trb_##NAME##_buf.count++, (0)))

/**
 * \brief   Push an element, overwriting the oldest element if the buffer is full
 *
 * \param   [in] NAME      Buffer name
 * \param   [in] VALUE_PTR Pointer to the element to be pushed
 */
#define trb_fifo_force_push(NAME, VALUE_PTR)\
    do {\
        memcpy(&_trb_##NAME##_buf.buf[_trb_##NAME##_buf.tail], VALUE_PTR, sizeof(_trb_##NAME##_buf.buf[0]));\
        _trb_##NAME##_buf.tail = (_trb_##NAME##_buf.tail + 1) % _trb_##NAME##_buf.capacity;\
        if (trb_is_full(NAME)) {\
            _trb_##NAME##_buf.head = (_trb_##NAME##_buf.head + 1) % _trb_##NAME##_buf.capacity;\
        } else {\
            _trb_##NAME##_buf.count++;\
        }\
    } while (0)

/**
 * \brief   Pop an element from the buffer
 *
 * \param   [in] NAME      Buffer name
 * \param   [in] VALUE_PTR Pointer where the popped element will be stored
 *
 * \return  - (0)  Success
 *          - (-1) Buffer empty
 */
#define trb_fifo_pop(NAME, VALUE_PTR)\
    (trb_is_empty(NAME)?(-1):\
    (memcpy(VALUE_PTR, &_trb_##NAME##_buf.buf[_trb_##NAME##_buf.head], sizeof(_trb_##NAME##_buf.buf[0])),\
    _trb_##NAME##_buf.head = (_trb_##NAME##_buf.head + 1) % _trb_##NAME##_buf.capacity,\
    _trb_##NAME##_buf.count--, (0)))

/**
 * \brief   Peek at the front element without removing it
 *
 * \param   [in] NAME      Buffer name
 * \param   [in] VALUE_PTR Pointer where the element will be copied
 *
 * \return  - (0)  Success
 *          - (-1) Buffer empty
 */
#define trb_fifo_peek(NAME, VALUE_PTR)\
    (trb_is_empty(NAME)?(-1):\
    (memcpy(VALUE_PTR, &_trb_##NAME##_buf.buf[_trb_##NAME##_buf.head], sizeof(_trb_##NAME##_buf.buf[0])),\
    (0)))

/**
 * \brief   Push an element into the buffer
 *
 * \param   [in] NAME      Buffer name
 * \param   [in] VALUE_PTR Pointer to the element to be pushed
 *
 * \return  - (0)  Success
 *          - (-1) Buffer full
 */
#define trb_lifo_push(NAME, VALUE_PTR)\
    (trb_is_full(NAME)?(-1):\
    (memcpy(&_trb_##NAME##_buf.buf[_trb_##NAME##_buf.head], VALUE_PTR, sizeof(_trb_##NAME##_buf.buf[0])),\
    _trb_##NAME##_buf.head++,\
    _trb_##NAME##_buf.count++, (0)))

/**
 * \brief   Pop an element from the buffer
 *
 * \param   [in] NAME      Buffer name
 * \param   [in] VALUE_PTR Pointer where the popped element will be stored
 *
 * \return  - (0)  Success
 *          - (-1) Buffer empty
 */
#define trb_lifo_pop(NAME, VALUE_PTR)\
    (trb_is_empty(NAME)?(-1):\
    (memcpy(VALUE_PTR, &_trb_##NAME##_buf.buf[_trb_##NAME##_buf.head - 1], sizeof(_trb_##NAME##_buf.buf[0])),\
    _trb_##NAME##_buf.head--,\
    _trb_##NAME##_buf.count--, (0)))

/**
 * \brief   Peek at the front element without removing it
 *
 * \param   [in] NAME      Buffer name
 * \param   [in] VALUE_PTR Pointer where the element will be copied
 *
 * \return  - (0)  Success
 *          - (-1) Buffer empty
 */
#define trb_lifo_peek(NAME, VALUE_PTR)\
    (trb_is_empty(NAME)?(-1):\
    (memcpy(VALUE_PTR, &_trb_##NAME##_buf.buf[_trb_##NAME##_buf.head - 1], sizeof(_trb_##NAME##_buf.buf[0])),\
    (0)))

#endif /* __TINY_RB_H__ */
