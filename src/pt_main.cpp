#include <args.hxx>

#include "renderers/path_tracing.h"
#include "utils/debug.h"
#include "utils/scene_parser.h"

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    args::ArgumentParser parser("ray tracer");

    args::HelpFlag help(parser, "HELP", "Show this help menu.", {"help"});
    args::ValueFlag<std::string> input(parser, "input_file", "input file", {'i', "input"}, "scenes/tmp.yml");
    args::ValueFlag<std::string> output(parser, "output_file", "output file", {'o', "output"}, "output/output.bmp");

    args::ValueFlag<int> subp(parser, "subp", "sub pixel", {'p', "subp"}, 1);
    args::ValueFlag<int> samples(parser, "samples", "samples", {'s', "samples"}, 1);

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help&) {
        std::cout << parser;
        return 0;
    }

    LOG(ERROR) << fmt::format("input: {}, output: {}", input.Get(), output.Get());

    RT::SceneParser scene_parser;
    scene_parser.parse(args::get(input));

    RT::PathTracingRender renderer(args::get(subp), args::get(samples), scene_parser);
    renderer.Render(*scene_parser.scene, *scene_parser.camera, args::get(output));
}
