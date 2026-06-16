#include <stdio.h>

#if 1
struct point_t {
  int x, y;
};
struct triangle_t {
  struct point_t pts[3];
};
#endif
/*
to find square of triangle frm its points coordinates: 
count det of the following matrix 
1/2 * det(| x1  y1  1 |)
          | x2  y2  1 |
          | x3  y3  1 | = x1 * y2 * 1 + x2 * y3 * 1 + x3 * y1 * 1 - ( x3 * y2 * 1 + x1 * y3 * 1 + x2 * y1 * 1)

*/   

int double_area(struct triangle_t tr){
    int res = tr.pts[0].x * tr.pts[1].y + tr.pts[1].x * tr.pts[2].y + tr.pts[2].x * tr.pts[0].y -
        (tr.pts[2].x * tr.pts[1].y + tr.pts[0].x * tr.pts[2].y + tr.pts[1].x * tr.pts[0].y);
        return res > 0 ? res : -res; 
}