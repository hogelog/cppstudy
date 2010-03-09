#include <iostream>
#include <boost/program_options.hpp>

#include "nqueen.hpp"

namespace options = boost::program_options;

void parse_options (options::variables_map& vmap, int argc, char *argv[])
{
    options::options_description opt("hidden option");
    options::positional_options_description pos_opt;
    opt.add_options()
        ("input", options::value<int>()->default_value(8), "N");
    pos_opt.add("input", -1);
    store(options::command_line_parser(argc, argv).options(opt).positional(pos_opt).run(), vmap);
}

int main (int argc, char *argv[])
{
    options::variables_map vmap;
    parse_options(vmap, argc, argv);

    int N = vmap["input"].as<int>();

    Board board(N);
    const Solution& sol = board.solve();
    std::cout <<
        "queens: " << N << "\n" <<
        "solutions: " << sol.size() << "\n";

    board.print_solution(sol);

    return 0;
}
