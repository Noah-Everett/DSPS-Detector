#############################################################################
####                    G4-DSPS-Detector-Simulation                      ####
#############################################################################
####                                                                     ####
#### Author:                                                             ####
####   Noah Everett (noah.everett@mines.sdsmt.edu)                       ####
####                                                                     ####
#### This file is based on the anniedirt Dockerfile created by me.       ####
#### https://github.com/ANNIEsoft/anniedirt/blob/genie_3_0_6/Dockerfile  ####
####                                                                     ####
#############################################################################
####                                                                     ####
#############################################################################



##############################
########## Set FROM ##########
##############################
FROM quay.io/centos/centos:stream8



##############################
########## Set User ##########
##############################
USER root



#######################################
########## Build Environment ##########
#######################################
RUN yum install -y dnf-plugins-core \
 && dnf config-manager --set-enabled extras,powertools \
 && yum install -y gcc \
                   gcc-c++ \
                   gcc-gfortran \
                   cmake \
                   make \
                   git \
                   wget \
                   python27 \
                   libxml2-devel \
                   gsl-devel \
                   quota \
                   patch \
                   which \
                   libnsl2-devel \
 && yum clean all \
 && rm -rf /vr/cache/yum \
 && wget https://raw.githubusercontent.com/Noah-Everett/DSPS-Detector/main/setup_Dockerfile.sh



#############################
########## log4cpp ##########
#############################
RUN source /setup_Dockerfile.sh \
 && git clone --depth 1 --branch v2.9.1 https://github.com/orocos-toolchain/log4cpp /log4cpp_src \
 && mkdir /log4cpp_build \
 && mkdir /log4cpp_install \
 && cd /log4cpp_build \
 && cmake /log4cpp_src -DCMAKE_INSTALL_PREFIX=/log4cpp_install \
 && make -j${NCPU} \
 && make install \
 && cp /log4cpp_install/lib/liborocos-log4cpp.so /log4cpp_install/lib/liblog4cpp.so \
 && cd / \
 && rm -rf /log4cpp_src \
 && rm -rf /log4cpp_build



############################
########## fsplit ##########
############################
RUN source /setup_Dockerfile.sh \
 && mkdir /fsplit \
 && cd /fsplit \
 && wget https://gist.githubusercontent.com/marc1uk/c0e32d955dd1c06ef69d80ce643018ad/raw/10e592d42737ecc7dca677e774ae66dcb5a3859d/fsplit.c \
 && gcc fsplit.c -o fsplit \
 && cd /



#############################
########## Pythia6 ##########
#############################
RUN source /setup_Dockerfile.sh \
 && git clone --depth 1 https://github.com/GENIE-MC-Community/Pythia6Support.git /Pythia6 \
 && cd /Pythia6 \
 && ./build_pythia6.sh --dummies=keep \
 && cd /



###########################
########## Expat ##########
###########################
RUN source /setup_Dockerfile.sh \
 && git clone --depth 1 --branch R_2_5_0 https://github.com/libexpat/libexpat /Expat_src \
 && mkdir /Expat_build \
 && mkdir /Expat_install \
 && cd /Expat_build \
 && cmake /Expat_src/expat -DCMAKE_INSTALL_PREFIX=/Expat_install \
 && make -j${NCPU} \
 && make install \
 && cd / \
 && rm -rf /Expat_src \
 && rm -rf /Expat_build



##############################
########## Xerces-C ##########
##############################
RUN source /setup_Dockerfile.sh \
 && git clone --depth 1 --branch v3.2.4 https://github.com/apache/xerces-c /Xerces-C_src \
 && mkdir /Xerces-C_build \
 && mkdir /Xerces-C_install \
 && cd /Xerces-C_build \
 && cmake /Xerces-C_src -DCMAKE_INSTALL_PREFIX=/Xerces-C_install \
 && make -j${NCPU} \
 && make install \
 && cd / \
 && rm -rf /Xerces-C_src \
 && rm -rf /Xerces-C_build



##########################
########## ROOT ##########
##########################
RUN source /setup_Dockerfile.sh \
 && git clone --depth 1 --branch v6-26-10 https://github.com/root-project/root /ROOT_src \
 && mkdir /ROOT_build \
 && mkdir /ROOT_install \
 && cd /ROOT_build \
 && cmake /ROOT_src -DCMAKE_INSTALL_PREFIX=/ROOT_install \
                    -Dx11=OFF \
                    -Dminimal=ON \
                    -Dgdml=ON \
                    -Dmathmore=ON \
                    -Dpythia6=ON \
                    -Dxml=ON \
 && make -j${NCPU} \
 && make install \
 && cd / \
 && rm -rf /ROOT_src \
 && rm -rf /ROOT_build



############################
########## Geant4 ##########
############################
RUN source /setup_Dockerfile.sh \
 && git clone --depth 1 --branch v11.2.1 https://github.com/Geant4/geant4 /Geant4_src \
 && mkdir /Geant4_build \
 && mkdir /Geant4_install \
 && cd /Geant4_build \
 && cmake /Geant4_src -DCMAKE_INSTALL_PREFIX=/Geant4_install \
                      -DCMAKE_PREFIX_PATH=/Xerces-C_install \
                      -DCMAKE_CXX_STANDARD=17 \
                      -DGEANT4_USE_GDML=ON \
                      -DGEANT4_USE_SYSTEM_EXPAT=OFF \
                      -DGEANT4_INSTALL_DATA=ON \
                      -DGEANT4_BUILD_MULTITHREADED=ON \
 && make -j${NCPU} \
 && make install \
 && cd / \
 && rm -rf /Geant4_src \
 && rm -rf /Geant4_build



###########################
########## GENIE ##########
###########################
RUN source /setup_Dockerfile.sh \
 && git clone --depth 1 https://github.com/ANNIEsoft/GENIE-v3 /GENIE \
 && cd /GENIE \
 && ./configure --prefix=/GENIE_install \
                --with-libxml2-inc=/usr/include/libxml2 \
                --with-libxml2-lib=/usr/lib64 \
                --with-pythia6-lib=/Pythia6/v6_424/lib \
                --with-log4cpp-inc=/log4cpp_install/include/oroco \
                --with-log4cpp-lib=/log4cpp_install/lib \
                --enable-fnal \
                --disable-lhapdf5 \
 && make -j${NCPU} \
 && make install \
 && cd / \
 && rm -rf /GENIE



##########################
########## NEST ##########
##########################
RUN source /setup_Dockerfile.sh \
 && git clone --depth 1 --branch v2.4.0 https://github.com/NESTCollaboration/nest /NEST_src \
 && mkdir /NEST_build \
 && mkdir /NEST_install \
 && cd /NEST_build \
 && cmake /NEST_src -DCMAKE_INSTALL_PREFIX=/NEST_install \
                    -DG4=ON \
                    -DBUILD_ROOT=ON \
 && make -j${NCPU} \
 && make install \
 && cd / \
 && rm -rf /NEST_src \
 && rm -rf /NEST_build



#############################
########## G4-DSPS ##########
#############################
RUN source /setup_Dockerfile.sh \
 && git clone --depth 1 https://github.com/Noah-Everett/DSPS-Detector /DSPS \
 && cd /DSPS \
 && mkdir build \
 && cd build \
 && cmake .. \
 && make -j${NCPU} \
 && cd /DSPS



############################
########## Python ##########
############################
RUN source /setup_Dockerfile.sh \
 && dnf install -y python3.11 \
                   python3-pip \
 && dnf clean all \
 && rm -rf /var/cache/dnf \
 && pip3 install numpy \
                 matplotlib \
                 pandas \
                 scikit-learn \
                 tensorflow \
                 uproot