#include <unordered_map>
#include <vector>
#include <utility>
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>

/*________________________________________________________________________________

MIT License

Copyright (c) 2026 Emeka Daniel

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

________________________________________________________________________________*/


/*
	The ReadMe file contains the documentation of this class
*/
class DataMapper {
	std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> m_dataMapper{};
	std::filesystem::path m_dataFile{};
	std::string m_outputFileContentsAsString{};
	bool m_dataMapperIsSet{};

public:
	DataMapper(std::filesystem::path dataFile) : m_dataFile{ std::move(dataFile) } {}

private:
	std::vector<std::string> readFile() {
		std::vector<std::string> line_contents_list{};
		std::string line_contents{};

		std::ifstream file{ m_dataFile };

		while (std::getline(file, line_contents)) {
			line_contents_list.push_back(std::move(line_contents));
		}

		return line_contents_list;
	}

	template<typename DataPair>
	void save_impl(DataPair data_pair) {
		std::ostringstream output_str_stream{};

		output_str_stream << data_pair.first << ": " << data_pair.second << "\n\n";

		m_outputFileContentsAsString += output_str_stream.str();
	}

	template<typename DataPair_1, typename... DataPairs>
	void save_impl(DataPair_1 data_pair_1, DataPairs... other_data_pairs) {
		std::ostringstream output_str_stream{};

		output_str_stream << data_pair_1.first << ": " << data_pair_1.second << '\n';

		m_outputFileContentsAsString += output_str_stream.str();

		save_impl(other_data_pairs...);
	}

	void setupDataMapper() {
		if (m_dataMapperIsSet) {
			return;
		}
		else {
			m_dataMapperIsSet = true;
		}

		auto fileContentsInList = readFile();

		auto isHeader = [](const std::string& str) -> bool {
			return str.starts_with("[[") and str.ends_with("]]");
			};

		for (size_t index{}, list_size = fileContentsInList.size(); index < list_size; index += 1) {
			if (isHeader(fileContentsInList[index])) {
				std::vector<std::pair<std::string, std::string>> dataList{};
				std::string header = std::move(fileContentsInList[index]);

				// Remove the first two markers
				header.erase(0, 2);
				// Remove the last two markers
				header.erase(header.size() - 2, 2);

				for (index += 1; ; index += 1) {
					if (index >= list_size or isHeader(fileContentsInList[index])) {
						m_dataMapper[header] = std::move(dataList);
						index -= 1;
						break;
					}

					auto semiColonIndex = fileContentsInList[index].find_first_of(':', 0);
					if (semiColonIndex != std::string::npos) {
						std::string string1(fileContentsInList[index].begin(), fileContentsInList[index].begin() + semiColonIndex);
						std::string string2(fileContentsInList[index].begin() + semiColonIndex + 2, fileContentsInList[index].end());

						dataList.push_back(std::pair{ std::move(string1), std::move(string2) });
					}
				}
			}
		}
	}

	template<typename DataPair>
	void retrive_impl(const std::vector<std::pair<std::string, std::string>>& dataPairList, DataPair data_pair) {
		for (auto&& dataPair : dataPairList) {
			if (dataPair.first == data_pair.first) {
				std::istringstream{ dataPair.second } >> *(data_pair.second);
			}
		}
	}

	template<typename DataPair_1, typename... DataPairs>
	void retrive_impl(const std::vector<std::pair<std::string, std::string>>& dataPairList, DataPair_1 data_pair_1, DataPairs... other_data_pairs) {
		for (auto&& dataPair : dataPairList) {
			if (dataPair.first == data_pair_1.first) {
				std::istringstream{ dataPair.second } >> *(data_pair_1.second);
			}
		}

		retrive_impl(dataPairList, other_data_pairs...);
	}

public:
	template<typename... DataPairs>
	void addDataHeaderSections(const std::string& header, DataPairs... data_pairs) {
		m_outputFileContentsAsString += "[[" + header + "]]\n";

		save_impl(data_pairs...);
	}

	void save() {
		std::ofstream{ m_dataFile } << m_outputFileContentsAsString;
	}

	template<typename... DataPairs>
	void retrieveDataHeaderSection(const std::string& header, DataPairs... data_pairs) {
		setupDataMapper();

		if (m_dataMapper.contains(header)) {
			retrive_impl(m_dataMapper[header], data_pairs...);
		}
	}
};
