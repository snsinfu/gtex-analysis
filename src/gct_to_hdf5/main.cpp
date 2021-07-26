#include <cstddef>
#include <iostream>
#include <istream>
#include <string>
#include <vector>

#include <h5.hpp>


struct table_metadata
{
    std::size_t rows;
    std::size_t columns;
};

static table_metadata           parse_metadata(std::istream& input);
static std::vector<std::string> parse_cells(std::istream& input);
static std::string              scan_token(std::string& str, char delim);


int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "usage: main <output>.h5\n";
        return 1;
    }

    h5::file output{argv[1], "w"};
    auto cells_dataset = output.dataset<h5::str, 1>("cells");
    auto codes_dataset = output.dataset<h5::str, 1>("codes");
    auto genes_dataset = output.dataset<h5::str, 1>("genes");
    auto exprs_dataset = output.dataset<h5::f32, 2>("expressions");

    auto& input = std::cin;

    auto const metadata = parse_metadata(input);
    auto const cells = parse_cells(input);
    std::vector<std::string> codes;
    std::vector<std::string> genes;
    std::vector<float> row;

    auto exprs_stream = exprs_dataset.stream_writer(
        {metadata.columns}, {.compression = 1}
    );

    for (std::string line; std::getline(std::cin, line); ) {
        codes.push_back(scan_token(line, '\t'));
        genes.push_back(scan_token(line, '\t'));

        row.clear();
        while (!line.empty()) {
            row.push_back(std::stof(scan_token(line, '\t')));
        }

        exprs_stream.write(row);
    }

    cells_dataset.write(cells);
    codes_dataset.write(codes);
    genes_dataset.write(genes);
}


table_metadata parse_metadata(std::istream& input)
{
    std::string line;

    while (std::getline(input, line) && line.starts_with('#')) {
    }

    table_metadata data;
    data.rows = std::stoul(scan_token(line, '\t'));
    data.columns = std::stoul(scan_token(line, '\t'));

    return data;
}


std::vector<std::string> parse_cells(std::istream& input)
{
    std::string line;
    std::getline(input, line);

    // The first two tokens are the names of metadata columns. Deop them.
    scan_token(line, '\t');
    scan_token(line, '\t');

    std::vector<std::string> cells;
    while (!line.empty()) {
        cells.push_back(scan_token(line, '\t'));
    }

    return cells;
}


std::string scan_token(std::string& str, char delim)
{
    std::string token;

    auto const pos = str.find(delim);
    if (pos != std::string::npos) {
        token = str.substr(0, pos);
        str.erase(0, pos + 1);
    } else {
        token = str;
        str.erase();
    }

    return token;
}
