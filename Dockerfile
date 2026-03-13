#FROM mythicproject/mythic_python_base:latest
From itsafeaturemythic/mythic_python_base:latest
RUN apt-get update && apt-get install -y gcc-mingw-w64
COPY . /Mythic/
WORKDIR /Mythic/
CMD ["python3", "main.py"]





