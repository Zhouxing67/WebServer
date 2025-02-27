#ifndef COPY_ABLE_H
#define COPY_ABLE_H

class copyable
{
  protected:
    copyable() = default;
    ~copyable() = default;
};

class noncopyable
{
  public:
    noncopyable(const noncopyable &) = delete;
    noncopyable &operator=(const noncopyable &) = delete;

  protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

class nonmovable
{
  public:
    nonmovable(nonmovable &&) = delete;
    nonmovable &operator=(nonmovable &&) = delete;

  protected:
    nonmovable() = default;
    ~nonmovable() = default;
};

class noncopyable_and_nonmoveable : public nonmovable, public noncopyable
{
};
#endif