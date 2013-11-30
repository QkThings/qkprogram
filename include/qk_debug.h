#ifndef QK_DEBUG_H
#define QK_DEBUG_H

#define _QK_DEBUGSTR_BUFSIZE  100

extern char _qk_debug_str[_QK_DEBUGSTR_BUFSIZE];

#ifndef QK_NO_DEBUG_OUTPUT
#define QK_DEBUG(...) {sprintf(_qk_debug_str, __VA_ARGS__); _qk_debug_sendString();} //FIXME inline this
#else
#define QK_DEBUG(...)
#endif

void _qk_debug(char *text, ...);
void _qk_debug_sendString();

#endif /* QK_DEBUG_H */
