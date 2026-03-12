# AUTHOR
The author's name is Daniel Emeka

# What Is DataMapper?
Is a class that manages the saving and retrieval of data from a file in a minimalist, but intuitive way. It stores the data in a specific serialization format not to different from mainstream data representation languages, like TOML and JSON.

# Why DataMapper?
DataMapper was concieved as a more than rudimentary approach to saving and retrieving of config data from the software projects I am working on. It is designed to make the developer's life easier by abstracting way the implementation details of pseudo-serialization

# Why Not A Formal Data Representaion Language?
Well, to be frank, I was just too lazy to download anyone of the numerous data representation languages out there and learn their documentation and so on. And since I felt it was relatively easy to do, I just went ahead to code one myself.

# Pros Of DataMapper
It is minimalist, so the setup required is near zero, just set the the filepath and your can begin saving and retrieving your config data

# Cons Of DataMapper
It uses unconstrained generic typing and the poorly designed variadic templates or parameter packs as it is called. So, invariants can't be met while using the class to save and retrieve data. Like everything in C++, just be extra careful when using the class

# How DataMapper Works
DataMapper works by diving data into sections with each header having it's own section called data header sections. It is models other data representation languages in that regard, and the reason for this design was symbolic to the specific project I am working on, as the data are split into various class or namespaces.

## DataMapper Functions

### `addDataHeaderSection`
**Parameter(header)**: The symbolic name of the data section pairs.

**Parameter(data_pairs)**: The actually data pairs to outputted, that is the _data's name_ and the _data itself_. A user can enter any number of data pairs they wish.

After calling a series of this function, it is required to call the `save` function to save the data header contents to the file.

Example:
```C++
DataMapper data_mapper{ std::filesystem::path{R"(C:\Users\Owner\Documents\dataFile.txt)"}};

data_mapper.addDataHeaderSection("Settings", std::pair{ "Boolean", true }, std::pair{ "Float", 9004.90f }, std::pair{ "Int", 788889 });

data_mapper.addDataHeaderSection("Other", std::pair{"Boolean_1", false}, std::pair{"Float_1", 50.90f}, std::pair{"Int_1", 99989});

data_mapper.save();
```

### `retrieveDataHeaderSection`
**Parameter(header)**: The symbolic name of the data section pairs.

**Parameter(data_pairs)**: The actually data pairs to be extracted to, that is the _data's name_ and the _pointer to the data's variable_. A user can enter any number of data pairs they wish.

And since this class uses a hash map to store the retrieved data header sections, data header sections with the same header coalesce into one data header section

Example:
```C++
bool booling, booling_1{};
float floating, floating_1{};
int inting, inting_1{};

data_mapper.retrieveDataHeaderSection("Settings", std::pair{"Boolean", &booling}, std::pair{"Float", &floating}, std::pair{"Int", &inting});

data_mapper.retrieveDataHeaderSection("Other", std::pair{"Boolean_1", &booling_1}, std::pair{"Float_1", &floating_1}, std::pair{"Int_1", &inting_1});
```

# Blah Blah Blah
Star this repository if you like and don't be shy to visit my `Nc` repository if interested

# Incase Of A Problem
Start an issue or something or you can contact me via email or telegram, mostly via telegram, though
