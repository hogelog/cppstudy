#include <vector>
#include <string>

typedef enum {
    PLACED, ABLE, DISABLE, UNKNOWN
} Cell;
typedef std::vector<Cell> BoardBuffer;

class Solution {
    private:
        size_t const size;
        std::vector<BoardBuffer *> solutions;
    public:
        Solution (size_t);
        Solution (Solution &);
        ~Solution ();
        size_t count() const;
        size_t add(BoardBuffer *);
        BoardBuffer * const & operator[](size_t) const;
        std::string const get_string(size_t) const;
};

class Board {
    private:
        BoardBuffer buffer;
        Solution solution;

    public:
        const size_t size;
        size_t queens;

        Board(size_t);
        Board(Board&);
        ~Board();
        void reset_buffer(size_t, int, int);
        Cell& operator() (int, int);
        bool placable (int, int);
        void place (int, int);
        const Solution& solve();
};
