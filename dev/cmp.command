# cd /users/garnetdev/desktop/sources/
# export PATH=$PATH:/users/garnetdev/desktop/sources/
# chmod u+x cmp.command
# ches cmp chestnut/src/Examples/hello.ches
# ches run chestnut/src/Examples/hello.chesc

rm ches
g++ -o ches chestnut/dev/src/main.cpp -std=c++1z -lstdc++ -lstdc++fs -std=c++17
