#include <args.hxx>

#include "renderers/photon_mapping.h"
#include "utils/debug.h"
#include "utils/scene_parser.h"

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    args::ArgumentParser parser("ray tracer");

    args::HelpFlag help(parser, "HELP", "Show this help menu.", {"help"});
    args::ValueFlag<std::string> input(parser, "input_file", "input file", {'i', "input"}, "scenes/cornell.yml");
    args::ValueFlag<std::string> output(parser, "output_file", "output file", {'o', "output"}, "output/output.bmp");

    args::ValueFlag<int> subp(parser, "subp", "sub pixel", {'p', "subp"}, 1);
    args::ValueFlag<int> samples(parser, "samples", "samples", {'s', "samples"}, 1);

    args::ValueFlag<float> alpha(parser, "alpha", "ppm alpha", {'a', "alpha"}, 0.5);

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help&) {
        std::cout << parser;
        return 0;
    }

    RT::SceneParser scene_parser;
    scene_parser.parse(args::get(input));

    RT::PhotonMappingRender renderer(
            scene_parser.gamma,
            alpha.Get(),
            scene_parser
    );
    renderer.Render(output.Get());
}
