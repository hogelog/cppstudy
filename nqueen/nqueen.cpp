#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <cmath>

#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/assert.hpp>

#include "nqueen.hpp"

typedef boost::tuple<int, int> Position;

inline static Position pos_prev(int size, int x, int y)
{
    if (--x == -1) {
        x = size-1;
        --y;
    }
    return boost::make_tuple(x, y);
}

inline static Position pos_next(int size, int x, int y)
{
    if (++x == size) {
        x = 0;
        ++y;
    }
    return boost::make_tuple(x, y);
}

inline static bool inboard(int size, int x, int y)
{
    return 0 <= x && x < size && 0 <= y && y < size;
}

inline static char cell2char(const Cell& cell)
{
    return (cell==PLACED ? '1' : (cell==ABLE ? '0' : (cell==DISABLE ? 'x' : 'u')));
}
inline static void print_buffer(int size, const BoardBuffer& buf)
{
    int col = 0;
    BOOST_FOREACH(const Cell& cell, buf) {
        std::cout << cell2char(cell) << (((++col%size)==0) ? '\n' : ' ');
    }
}


Board::Board(int n) :
    buffer(n*n, ABLE),
    solution(0),
    size(n),
    queens(0)
{
}

Board::Board(Board& board) :
    buffer(board.size*board.size),
    solution(0),
    size(board.size),
    queens(board.queens)
{
    for (int y=0;y<size;++y) {
        for (int x=0;x<size;++x) {
            operator()(x, y) = board(x, y);
        }
    }
}

Board::~Board()
{
    BOOST_FOREACH(BoardBuffer * const &buf, solution) {
        delete buf;
    }
}

void Board::reset_buffer(int queens, int x, int y)
{
    this->queens = queens;
    for (int cx=x, cy=y;inboard(size, cx, cy);boost::tie(cx, cy)=pos_next(size, cx, cy)) {
        operator()(cx, cy) = UNKNOWN;
    }
    for (int cx=x, cy=y;inboard(size, cx, cy);boost::tie(cx, cy)=pos_next(size, cx, cy)) {
        operator()(cx, cy) = placable(cx, cy) ? ABLE : DISABLE;
    }
}

Cell& Board::operator() (int x, int y)
{
    assert(inboard(size, x, y));
    return buffer[y*size + x];
}

bool Board::placable (int x, int y)
{
    for (int cy=0;cy<size;++cy) {
        for (int cx=0;cx<size;++cx) {
            if (operator()(cx, cy)==PLACED) {
                if (cx == x || cy == y || abs(cy-y) == abs(cx-x))
                    return false;
            }
        }
    }
    return true;
}

void Board::place (int x, int y)
{
    assert(operator()(x, y)==ABLE);
    for (int cx=0;cx<size;++cx) {
        operator()(cx, y) = cx==x ? PLACED : DISABLE;
    }
    for (int cy=y+1;cy<size;++cy) {
        for (int cx=0;cx<size;++cx) {
            operator()(cx, cy) = placable(cx, cy) ? ABLE : DISABLE;
        }
    }
}

const Solution& Board::solve()
{
    if (solution.size()!=0)
        return solution;

    std::stack<boost::tuple<int, int, int> > stack;
    int queens = 0, x = 0, y = 0;
    int trying = 0;
    for (;;) {
        ++trying;
        int nx, ny;
        for (; inboard(size, x, y); x = nx, y = ny) {
            boost::tie(nx, ny) = pos_next(size, x, y);
            if (operator()(x, y)==ABLE) {
                if (inboard(size, nx, ny))
                    stack.push(boost::make_tuple(queens, nx, ny));
                place(x, y);
                if (++queens == size) {
                    solution.push_back(new BoardBuffer(buffer));
                    break;
                }
            }
        }

        if (stack.size() == 0)
            break;
        boost::tie(queens, x, y) = stack.top();
        assert(inboard(size, x, y));
        stack.pop();

        int px, py;
        boost::tie(px, py) = pos_prev(size, x, y);
        assert(inboard(size, px, py));
        reset_buffer(queens, px, py);
    }

    return solution;
}

int Board::print_solution(const Solution& solution)
{
    int solutions = 0;
    BOOST_FOREACH(BoardBuffer * const &buf, solution) {
        std::cout << boost::format("solution %1%:\n") % (++solutions);
        print_buffer(size, *buf);
        std::cout << std::endl;
    }
    return solution.size();
}
