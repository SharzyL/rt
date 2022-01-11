#include <args.hxx>

#include "renderers/photon_mapping.h"
#include "utils/debug.h"
#include "utils/scene_parser.h"

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    args::ArgumentParser parser("ray tracer");

    args::HelpFlag help(parser, "HELP", "Show this help menu.", {"help"});
    args::ValueFlag<std::string> input(parser, "input_file", "input file", {'i', "input"}, "scenes/cornell.yml");
    args::ValueFlag<std::string> output(parser, "output_file", "output file", {'o', "output"}, "output/output-sppm.bmp");

    args::ValueFlag<int> vp_per_pixel(parser, "vp", "vp", {'v', "vp"}, 1);
    args::ValueFlag<int> photons_per_round(parser, "photons", "photons per round", {'p', "photons"}, 10000);
    args::ValueFlag<int> num_rounds(parser, "rounds", "rounds", {'n', "rounds"}, 5);

    args::ValueFlag<float> alpha(parser, "alpha", "ppm alpha", {'a', "alpha"}, 0.7);
    args::ValueFlag<float> init_radius(parser, "init-radius", "init radius", {'r', "radius"}, 0.001);

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help&) {
        std::cout << parser;
        return 0;
    }

    LOG(ERROR) << fmt::format("input: {}, output: {}", input.Get(), output.Get());

    RT::SceneParser scene_parser;
    scene_parser.parse(args::get(input));

    RT::PhotonMappingRender renderer(
            alpha.Get(),
            init_radius.Get(),
            num_rounds.Get(),
            photons_per_round.Get(),
            vp_per_pixel.Get(),
            scene_parser
    );
    renderer.Render(output.Get());
}
