#include <args.hxx>

#include "core/camera.h"
#include "core/renderer.h"
#include "debug.h"
#include "util.h"

#include "objects/group.h"

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    args::ArgumentParser parser("ray tracer");

    args::HelpFlag help(parser, "HELP", "Show this help menu.", {"help"});
    args::ValueFlag<std::string> input(parser, "input_file", "input file", {'i', "input"},
                                       "scenes/CornellBox-Original.obj");
    args::ValueFlag<std::string> search_path(parser, "path", "material search path", {'I'}, "scenes");
    args::ValueFlag<std::string> output(parser, "output_file", "output file", {'o', "output"}, "output/output.bmp");

    args::ValueFlag<std::string> cam_pos(parser, "cam_pos", "cam pos", {"cam-pos"}, "0, 0, 10");
    args::ValueFlag<std::string> cam_dir(parser, "cam_dir", "cam dir", {"cam-dir"}, "0, 0, -1");
    args::ValueFlag<std::string> cam_up(parser, "cam_up", "cam up", {"cam-up"}, "0, 1, 0");
    args::ValueFlag<int> width(parser, "width", "canvas width", {'w', "width"}, 320);
    args::ValueFlag<int> height(parser, "height", "canvas height", {'h', "height"}, 320);
    args::ValueFlag<float> angle(parser, "angle", "camera angle", {'a', "angle"}, 45.f);

    args::ValueFlag<int> subp(parser, "subp", "sub pixel", {'p', "subp"}, 1);
    args::ValueFlag<int> samples(parser, "samples", "samples", {'s', "samples"}, 1);
    args::ValueFlag<float> gamma(parser, "gamma", "gamma value", {'g', "gamma"}, 2.2f);

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help&) {
        std::cout << parser;
        return 0;
    }

    PerspectiveCamera camera = PerspectiveCamera(
            parse_vector(args::get(cam_pos)),
            parse_vector(args::get(cam_dir)),
            parse_vector(args::get(cam_up)),
            args::get(width),
            args::get(height),
            to_radian(args::get(angle))
    );
    Group g(args::get(input), args::get(search_path));

    Renderer renderer(args::get(subp), args::get(samples), args::get(gamma));
    renderer.Render(g, camera, args::get(output));
}
