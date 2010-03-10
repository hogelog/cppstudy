#include <stack>

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

Solution::Solution (size_t size) :
    size(size),
    solutions(0)
{
}

Solution::Solution (Solution& sol) :
    size(sol.size),
    solutions(sol.solutions)
{
}

Solution::~Solution ()
{
    BOOST_FOREACH (BoardBuffer * const buf, solutions) {
        delete buf;
    }
    solutions.clear();
}

size_t Solution::count () const
{
    return solutions.size();
}

size_t Solution::add (BoardBuffer * const buf)
{
    solutions.push_back(buf);
    return solutions.size();
}

BoardBuffer * const & Solution::operator[] (size_t num) const
{
    assert(0 <= num && num < count());
    return solutions[num];
}

std::string const Solution::get_string(size_t num) const
{
    assert(0 <= num && num < count());
    BoardBuffer * const buf = solutions[num];

    std::string str;
    size_t col = 0;
    BOOST_FOREACH(const Cell& cell, *buf) {
        str += cell2char(cell);
        str += ((++col % size) == 0 ? '\n' : ' ');
    }

    return str;
}

Board::Board(size_t n) :
    buffer(n*n, ABLE),
    solution(n),
    size(n),
    queens(0)
{
}

Board::Board(Board& board) :
    buffer(board.size*board.size),
    solution(board.size),
    size(board.size),
    queens(board.queens)
{
    for (int y=0;y<static_cast<int>(size);++y) {
        for (int x=0;x<static_cast<int>(size);++x) {
            operator()(x, y) = board(x, y);
        }
    }
}

Board::~Board()
{
}

void Board::reset_buffer(size_t queens, int x, int y)
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
    for (int cy=0;cy<static_cast<int>(size);++cy) {
        for (int cx=0;cx<static_cast<int>(size);++cx) {
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
    for (int cx=0;cx<static_cast<int>(size);++cx) {
        operator()(cx, y) = cx==x ? PLACED : DISABLE;
    }
    for (int cy=y+1;cy<static_cast<int>(size);++cy) {
        for (int cx=0;cx<static_cast<int>(size);++cx) {
            operator()(cx, cy) = placable(cx, cy) ? ABLE : DISABLE;
        }
    }
}

const Solution& Board::solve ()
{
    if (solution.count()!=0)
        return solution;

    std::stack<boost::tuple<int, int, int> > stack;
    size_t queens = 0;
    int x = 0, y = 0;
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
                    solution.add(new BoardBuffer(buffer));
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
