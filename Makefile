output_file := app.exe
files := ./src/*.cpp ./src/Physics/*.cpp
flags := -lm -lSDL2 -lSDL2_image -lSDL2_gfx

build:
	@g++ -std=c++17 -Wall $(files) $(flags) -o $(output_file)
	@echo "build successful"

run: build
	@./$(output_file)

clean:
	@rm $(output_file)
