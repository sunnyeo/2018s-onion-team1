# Use ubuntu as parent image
FROM ubuntu

# Set the working directory into the container at /onion
WORKDIR /onion

# Add the current directory contents into the container at /onion
ADD . /onion

# Don we need to install any dependicies?
# RUN install-dependencies

# Run the onion messenger using Makefile(not created yet)
CMD ["make"]
