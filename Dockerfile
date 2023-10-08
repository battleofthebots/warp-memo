FROM ghcr.io/battleofthebots/botb-base-image:ubuntu-20.04-defcon-2023

USER user

WORKDIR /home/user

COPY notes.c .
COPY Makefile .

RUN make

RUN rm notes.c Makefile

EXPOSE 1337

ENTRYPOINT [ "/home/user/warp-memo" ]