#include <iostream>
#include <boost/format.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/program_options.hpp>

#include "nqueen.hpp"

namespace options = boost::program_options;
typedef boost::tuple<options::options_description * const, options::variables_map * const> Option;
Option parse_options (int argc, char *argv[])
{
    static options::variables_map vmap;
    static options::options_description opt("options");
    opt.add_options()
        ("help,h", "print this help")
        ("all,a", "print all solutions");

    options::options_description hopt("hidden options");
    hopt.add_options()
        ("input", options::value<int>()->default_value(8), "N");

    options::positional_options_description pos_hopt;
    pos_hopt.add("input", -1);

    options::command_line_parser parser(argc, argv);
    options::options_description all_opt("all options");

    hopt.add(opt);
    parser.options(hopt).positional(pos_hopt);
    options::store(parser.run(), vmap);

    return boost::make_tuple(&opt, &vmap);
}

int main (int argc, char *argv[])
{
    options::options_description * opt;
    options::variables_map * vmap;
    boost::tie(opt, vmap) = parse_options(argc, argv);

    if (vmap->count("help")) {
        std::cout <<
            (boost::format("%1% [options] [N = 8]\n") % argv[0]) <<
            *opt << std::endl;
        exit(0);
    }

    int N = (*vmap)["input"].as<int>();

    Board board(N);
    const Solution& sol = board.solve();
    std::cout <<
        "queens: " << N << "\n" <<
        "solutions: " << sol.size() << "\n";

    if (vmap->count("all"))
        board.print_solution(sol);

    return 0;
}
