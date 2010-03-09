typedef enum {
    PLACED, ABLE, DISABLE, UNKNOWN
} Cell;
typedef std::vector<Cell> BoardBuffer;
typedef std::vector<BoardBuffer*> Solution;

class Board {
    private:
        BoardBuffer buffer;
        Solution solution;

    public:
        const int size;
        int queens;

        Board(int);
        Board(Board&);
        ~Board();
        void reset_buffer(int, int, int);
        Cell& operator() (int, int);
        bool placable (int, int);
        void place (int, int);
        const Solution& solve();
        int print_solution(const Solution&);
};
