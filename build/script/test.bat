python unpack.py -i ../../3rdparty/ -o ./_unpack -c ../build_config.xml
python boost.py -c ../build_config.xml -t ./_unpack/boost/inlcudes -l ./_unpack/boost/libs -r