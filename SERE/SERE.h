// SERE.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

#include <fstream>
#include <streambuf>
#include <execution>
#include <system_error>
#include <atomic>

#include <iostream>
#include <optional>
#include "RenderFrameworks/RenderFramework.h"

namespace {

    struct CommandLineOptions {
        bool showHelp = false;
        bool exportGraph = false;
        fs::path graphInputPath;
        fs::path exportOutputPath;
        std::optional<int> width;
        std::optional<int> height;
        std::optional<std::string> gamePath;
        std::optional<std::string> customRpakPath;
        std::string error;
    };

    class HeadlessRenderFramework final : public RenderFramework {
    public:
        bool ShouldMainLoopRun() override { return false; }
        bool ImGuiStartFrame() override { return false; }
        void ImGuiEndFrame() override {}
        void ImGuiDeInit() override {}

        size_t LoadTexture(fs::path&) override { return NextResourceId(); }
        size_t CreateTextureFromData(void*, uint32_t, uint32_t, uint16_t, uint32_t, uint32_t) override { return NextResourceId(); }
        size_t CreateShaderDataBuffer(std::vector<ShaderSizeData_t>) override { return NextResourceId(); }

        void RuiWriteIndexBuffer(std::vector<uint16_t>&) override {}
        void RuiWriteVertexBuffer(std::vector<Vertex_t>&) override {}
        void RuiWriteStyleBuffer(std::vector<StyleDescriptorShader_t>&) override {}
        void RuiClearFrame() override {}
        void DrawIndexed(uint32_t, uint32_t, size_t*) override {}
        void RuiBindPipeline() override {}
        void RuiLoad(int, int) override {}
        void RuiReCreatePipeline(int, int) override {}

        void* GetTextureView(size_t) override { return nullptr; }
        void* GetRuiView() override { return nullptr; }
        void* GetWindow() override { return nullptr; }

    private:
        size_t NextResourceId() { return nextResourceId.fetch_add(1); }

        std::atomic_size_t nextResourceId{ 1 };
    };

    bool TryParsePositiveInt(const std::string& text, int& value)
    {
        try {
            size_t parsed = 0;
            int parsedValue = std::stoi(text, &parsed);
            if (parsed != text.size() || parsedValue <= 0)
                return false;

            value = parsedValue;
            return true;
        }
        catch (...) {
            return false;
        }
    }

    bool ReadValueOption(int argc, char** argv, int& index, const std::string& optionName, std::string& value, std::string& error)
    {
        if (index + 1 >= argc) {
            error = optionName + " expects a value.";
            return false;
        }

        value = argv[++index];
        return true;
    }


    CommandLineOptions ParseCommandLine(int argc, char** argv)
    {
        CommandLineOptions options;

        for (int i = 1; i < argc; i++) {
            const std::string arg = argv[i];

            if (arg == "--help" || arg == "-h" || arg == "/?") {
                options.showHelp = true;
                continue;
            }

            if (arg == "--export-graph") {
                if (options.exportGraph) {
                    options.error = "--export-graph can only be specified once.";
                    return options;
                }
                if (i + 2 >= argc) {
                    options.error = "--export-graph expects <input.json> <output.ruip>.";
                    return options;
                }

                options.exportGraph = true;
                options.graphInputPath = argv[++i];
                options.exportOutputPath = argv[++i];
                continue;
            }

            if (arg == "--width" || arg == "--height") {
                std::string valueText;
                if (!ReadValueOption(argc, argv, i, arg, valueText, options.error))
                    return options;

                int value = 0;
                if (!TryParsePositiveInt(valueText, value)) {
                    options.error = arg + " expects a positive integer.";
                    return options;
                }

                if (arg == "--width")
                    options.width = value;
                else
                    options.height = value;
                continue;
            }

            if (arg == "--size") {
                std::string widthText;
                std::string heightText;
                if (!ReadValueOption(argc, argv, i, arg, widthText, options.error))
                    return options;
                if (!ReadValueOption(argc, argv, i, arg, heightText, options.error))
                    return options;

                int width = 0;
                int height = 0;
                if (!TryParsePositiveInt(widthText, width) || !TryParsePositiveInt(heightText, height)) {
                    options.error = "--size expects two positive integers.";
                    return options;
                }

                options.width = width;
                options.height = height;
                continue;
            }

            if (arg == "--game-path" || arg == "--custom-rpak-path") {
                std::string value;
                if (!ReadValueOption(argc, argv, i, arg, value, options.error))
                    return options;

                if (arg == "--game-path")
                    options.gamePath = value;
                else
                    options.customRpakPath = value;
                continue;
            }

            if (arg == "-dx11" || arg == "-ogl")
                continue;

            if (options.exportGraph) {
                options.error = "Unknown option: " + arg;
                return options;
            }
        }

        return options;
    }

    void PrintCommandLineUsage()
    {
        std::cout
            << "Usage:\n"
            << "  SERE --export-graph <input.json> <output.ruip> [options]\n\n"
            << "Options:\n"
            << "  --game-path <folder>         Titanfall 2 folder used for base RPak assets.\n"
            << "  --custom-rpak-path <folder>  Folder of custom .rpak files to load.\n"
            << "  --width <pixels>             Override exported RUI width.\n"
            << "  --height <pixels>            Override exported RUI height.\n"
            << "  --size <width> <height>      Override both exported RUI dimensions.\n";
    }
}