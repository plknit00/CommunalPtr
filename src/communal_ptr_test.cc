#include <memory>
#include <utility>

#include "gtest/gtest.h"

#include "src/communal_ptr_test_fixture.h"

TYPED_TEST_P(CommunalPtrTest, Uninitialized) {
  using PtrT = CommunalPtrTest<TypeParam>::template PtrT<uint64_t>;
  PtrT shared;

  EXPECT_EQ(shared.get(), nullptr);
  EXPECT_EQ(shared.use_count(), 0);
}

TYPED_TEST_P(CommunalPtrTest, Construct) {
  auto shared = this->template MakeShared<uint64_t>(1022);
  EXPECT_EQ(*shared, 1022);
}

TYPED_TEST_P(CommunalPtrTest, Destroy) {
  std::shared_ptr<bool> destructor_flag;
  {
    auto shared = this->template MakeShared<DestructorFlag<uint64_t>>();
    destructor_flag = shared->GetDestroyedFlag();
    // shared should be deleted.
  }
  EXPECT_TRUE(*destructor_flag);
}

TYPED_TEST_P(CommunalPtrTest, Copy) {
  auto shared = this->template MakeShared<DestructorFlag<uint64_t>>();
  void* ptr = shared.get();
  auto destroyed_flag = shared->GetDestroyedFlag();
  auto copy = shared;

  EXPECT_EQ(shared.get(), ptr);
  EXPECT_EQ(copy.get(), ptr);
  EXPECT_EQ(shared.use_count(), 2);
  EXPECT_EQ(copy.use_count(), 2);
  EXPECT_FALSE(*destroyed_flag);
}

TYPED_TEST_P(CommunalPtrTest, UseCopyAfterOriginalDestroyed) {
  using PtrT =
      CommunalPtrTest<TypeParam>::template PtrT<DestructorFlag<uint64_t>>;
  std::shared_ptr<bool> destructor_flag;
  {
    PtrT copy;
    void* ptr;

    {
      auto shared = this->template MakeShared<DestructorFlag<uint64_t>>();
      ptr = shared.get();
      destructor_flag = shared->GetDestroyedFlag();
      copy = shared;
      // shared should be deleted.
    }

    EXPECT_EQ(copy.get(), ptr);
    EXPECT_EQ(copy.use_count(), 1);
    ASSERT_FALSE(*destructor_flag);
  }
  EXPECT_TRUE(*destructor_flag);
}

TYPED_TEST_P(CommunalPtrTest, Move) {
  auto shared = this->template MakeShared<DestructorFlag<uint64_t>>();
  void* ptr = shared.get();
  auto destroyed_flag = shared->GetDestroyedFlag();
  auto copy = std::move(shared);

  EXPECT_EQ(shared.get(), nullptr);
  EXPECT_EQ(copy.get(), ptr);
  EXPECT_EQ(shared.use_count(), 0);
  EXPECT_EQ(copy.use_count(), 1);
  EXPECT_FALSE(*destroyed_flag);
}

TYPED_TEST_P(CommunalPtrTest, UseMoveAfterOriginalDestroyed) {
  using PtrT =
      CommunalPtrTest<TypeParam>::template PtrT<DestructorFlag<uint64_t>>;
  std::shared_ptr<bool> destructor_flag;
  {
    PtrT copy;
    void* ptr;

    {
      auto shared = this->template MakeShared<DestructorFlag<uint64_t>>();
      ptr = shared.get();
      destructor_flag = shared->GetDestroyedFlag();
      copy = std::move(shared);
      // shared should be deleted.
    }

    EXPECT_EQ(copy.get(), ptr);
    EXPECT_EQ(copy.use_count(), 1);
    ASSERT_FALSE(*destructor_flag);
  }
  EXPECT_TRUE(*destructor_flag);
}

REGISTER_TYPED_TEST_SUITE_P(CommunalPtrTest, Uninitialized, Construct, Destroy,
                            Copy, UseCopyAfterOriginalDestroyed, Move,
                            UseMoveAfterOriginalDestroyed);

INSTANTIATE_TYPED_TEST_SUITE_P(CommunalPtr, CommunalPtrTest,
                               TemplateWrapper<std::shared_ptr>);
// TODO: uncomment and remove previous line to test your code.
// INSTANTIATE_TYPED_TEST_SUITE_P(CommunalPtr, CommunalPtrTest,
//                                TemplateWrapper<std::shared_ptr>,
//                                TemplateWrapper<paige::CommunalPtr>);
