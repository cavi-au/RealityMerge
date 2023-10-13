#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

// third-party
#include <cavi/usdj_am/file.hpp>
#include <cavi/usdj_am/utils/document.hpp>
#include <cavi/usdj_am/utils/item.hpp>
#include <cavi/usdj_am/utils/json_writer.hpp>

// local
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

using std::filesystem::exists;
using std::filesystem::path;
using std::filesystem::temp_directory_path;

path const ASSETS = "assets";

path const ROOT = "files";

TEST_CASE("Load a USDJ-AM file", "[File]") {
    using namespace cavi::usdj_am;

    CHECK_THROWS_AS(utils::Document::load(path{}), std::invalid_argument);
    auto document = utils::Document::load(ROOT / ASSETS / "helloWorld.usdj-am");
    CHECK(document != static_cast<AMdoc*>(nullptr));
    CHECK_THROWS_AS(File{nullptr}, std::invalid_argument);
    auto file = File{document};
}

TEST_CASE("Validate `File` with USDA.JSON files", "[File]") {
    using namespace cavi::usdj_am;

    auto STEM =
        GENERATE(as<std::string>{}, "Ball.shadingVariants", "helloWorld", "relativeReference", "usdPhysicsBoxOnBox");
    auto usdj_am_path = ROOT / ASSETS / (STEM + ".usdj-am");
    auto document = utils::Document::load(usdj_am_path);
    CHECK(document != static_cast<AMdoc*>(nullptr));
    auto file = File{document};
    // Match the indenting of the example USDA JSON files.
    utils::JsonWriter json_writer{utils::JsonWriter::Indenter{' ', 2}};
    file.accept(json_writer);
    auto usda_json_path = ROOT / ASSETS / (STEM + ".usda.json");
    std::ifstream ifs(usda_json_path, std::ios::in | std::ios::ate);
    CHECK(ifs);
    std::string usda_json;
    usda_json.reserve(ifs.tellg());
    ifs.seekg(0, std::ios::beg);
    usda_json.assign(std::istreambuf_iterator<std::ifstream::char_type>(ifs),
                     std::istreambuf_iterator<std::ifstream::char_type>());
    CHECK(json_writer.operator std::string() == usda_json);
}

TEST_CASE("Validate nested `File` with USDA.JSON files", "[File]") {
    using namespace cavi::usdj_am;

    auto STEM = GENERATE(as<std::string>{}, "brave-ape-49");
    auto usdj_am_path = ROOT / (STEM + ".automerge");
    auto document = utils::Document::load(usdj_am_path);
    CHECK(document != static_cast<AMdoc*>(nullptr));
    auto file = File{document, document.get_root() / "data" / "scene"};
    // Match the indenting of the example USDA JSON files.
    utils::JsonWriter json_writer{utils::JsonWriter::Indenter{' ', 2}};
    file.accept(json_writer);
    // Format the canonical JSON file with jq.
    path const TEMP = temp_directory_path();
    auto usda_json_path = ROOT / (STEM + ".usda.json");
    auto lhs_jq_json_path = TEMP / (STEM + ".lhs.jq.json");
    std::ostringstream command;
    command << "jq -S . " << usda_json_path << " > " << lhs_jq_json_path;
    CHECK(system(command.str().c_str()) == 0);
    CHECK(exists(lhs_jq_json_path));
    // Format the JSON output with jq.
    auto usdj_am_json_path = TEMP / (STEM + ".usdj-am.json");
    std::ofstream ofs(usdj_am_json_path, std::ios::out);
    CHECK(ofs);
    ofs << json_writer.operator std::string();
    ofs.close();
    CHECK(exists(usdj_am_json_path));
    auto rhs_jq_json_path = TEMP / (STEM + ".rhs.jq.json");
    command.str("");
    command << "jq -S . " << usdj_am_json_path << " > " << rhs_jq_json_path;
    CHECK(system(command.str().c_str()) == 0);
    CHECK(exists(rhs_jq_json_path));
    // Compare the formatted JSON files.
    std::ifstream lhs_fs(lhs_jq_json_path, std::ios::in | std::ios::ate);
    CHECK(lhs_fs);
    std::string lhs_jq_json;
    lhs_jq_json.reserve(lhs_fs.tellg());
    lhs_fs.seekg(0, std::ios::beg);
    lhs_jq_json.assign(std::istreambuf_iterator<std::ifstream::char_type>(lhs_fs),
                       std::istreambuf_iterator<std::ifstream::char_type>());
    std::ifstream rhs_fs(rhs_jq_json_path, std::ios::in | std::ios::ate);
    CHECK(rhs_fs);
    std::string rhs_jq_json;
    rhs_jq_json.reserve(rhs_fs.tellg());
    rhs_fs.seekg(0, std::ios::beg);
    rhs_jq_json.assign(std::istreambuf_iterator<std::ifstream::char_type>(rhs_fs),
                       std::istreambuf_iterator<std::ifstream::char_type>());
    CHECK(lhs_jq_json == rhs_jq_json);
}
