#ifndef ERROR_TYPE_HPP
#define ERROR_TYPE_HPP

#include <string>

enum class ErrorCode
{
    Success = 0,

    // 文件类错误
    FileNotFound,          // 未找到文件
    FilePermissionDenied,  // 文件访问被拒绝

    // 数据类错误
    JsonParseError,        // JSON解析错误
    MissingRequiredField,  // 缺失需求部分
    DataTypeMismatch,      // 数据类型不匹配
    DuplicateId,           // 重复的ID

    // 逻辑类错误
    ResourceNotFound,  // 源头不存在
    InvalidOperation   // 非法操作
};

inline std::string_view to_string(ErrorCode code)
{
    switch (code)
    {
        case ErrorCode::Success:
            return "操作成功";
        case ErrorCode::FileNotFound:
            return "找不到指定的数据文件";
        case ErrorCode::JsonParseError:
            return "JSON 语法解析失败";
        case ErrorCode::MissingRequiredField:
            return "数据项缺少必要字段";
        default:
            return "未知错误";
    }
}

#endif