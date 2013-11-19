#include "passthroughpainter.h"

PassthroughPainter::PassthroughPainter(QPainter &target)
    : inner(target)
{
}
