output_file := app.exe
files := ./src/*.cpp ./src/Physics/*.cpp
flags := -lm -lSDL2 -lSDL2_image -lSDL2_gfx

files:
	@g++ -std=c++17 -Wall $(files) $(flags) -o $(output_file)
	@echo "build successful"

build: files

run:
	@./$(output_file)

test: build run

clean:
	@rm $(output_file)
