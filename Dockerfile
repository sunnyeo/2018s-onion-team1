# Use GCC compiler as a parent image
FROM ubuntu

# Set the working directory into the container at /onion
WORKDIR /onion

# Add the current directory contents into the container at /onion
# User's private key is also transfered, since it is stored in the same directory with Dockerfile
ADD . /onion

# Don we need to install any dependicies?
RUN apt-get update
RUN apt-get -y install gcc \
                       netcat

# Compile the onion messenger using Makefile(not created yet)
CMD ["make"]
