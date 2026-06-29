SUPPORTED_CONTRACT_VERSION = 1
SUPPORTED_CONTRACT_VERSIONS = {SUPPORTED_CONTRACT_VERSION}

CURRENT_SAVE_FORMAT_VERSION = 1
SUPPORTED_SAVE_FORMAT_VERSIONS = {CURRENT_SAVE_FORMAT_VERSION}


def is_supported_contract_version(version: int) -> bool:
    return version in SUPPORTED_CONTRACT_VERSIONS


def is_supported_save_format_version(version: int) -> bool:
    return version in SUPPORTED_SAVE_FORMAT_VERSIONS