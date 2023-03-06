--[[
Author: mousechannel mochenghh@gmail.com
Date: 2023-03-05 11:17:39
LastEditors: mousechannel mochenghh@gmail.com
LastEditTime: 2023-03-05 11:18:33
FilePath: \nvim\lua\plugins\lsp.lua
Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
--]]
require("mason").setup({
    ui = {
        icons = {
            package_installed = "✓",
            package_pending = "➜",
            package_uninstalled = "✗"
        }
    }
})
require("mason-lspconfig").setup({
  -- 确保安装，根据需要填写
--  ensure_installed = {
  --  "lua_ls",
  --},
})
require'lspconfig'.clangd.setup({})

